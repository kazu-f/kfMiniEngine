#include "stdafx.h"
#include "Tonemap.h"

namespace Engine {
	namespace {
		const wchar_t* SHADER_FILEPATH = L"Assets/shader/tonemap.fx";

		//-----------------------------------------------------------------------------
		// Name: GetSampleOffsets_DownScale4x4
		// Desc: Get the texture coordinate offsets to be used inside the DownScale4x4
		//       pixel shader.
		//-----------------------------------------------------------------------------
		HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, Vector4 avSampleOffsets[])
		{
			if (NULL == avSampleOffsets)
				return E_INVALIDARG;

			float tU = 1.0f / dwWidth;
			float tV = 1.0f / dwHeight;

			// Sample from the 16 surrounding points. Since the center point will be in
			// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
			// center.
			int index = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					avSampleOffsets[index].x = (x - 1.5f) * tU;
					avSampleOffsets[index].y = (y - 1.5f) * tV;

					index++;
				}
			}

			return S_OK;
		}
	}

	void CTonemap::Init(const SPostEffectConfig& config)
	{
		if (!config.tonemap.isEnable)
		{
			ENGINE_LOG("トーンマップを適用しない。")
			return;
		}

		//レンダリングターゲットの作成。
		CreateRenderTargets();

		//ルートシグネチャの作成。
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);

		//シェーダーの初期化。
		InitShader();

		//パイプラインステートの作成。
		InitPipelineState();

		//定数バッファの作成。
		CreateConstantBuffer();

		//ディスクリプタヒープの作成。
		CreateDescriptorHeap();

		m_isEnable = config.tonemap.isEnable;
		SetLuminance(config.tonemap.luminance);
	}

	void CTonemap::CreateRenderTargets()
	{
		//輝度抽出用のレンダリングターゲット作成。
		for (int i = 0; i < NUM_CALC_AVG_RT; i++)
		{
			int rtSize = 1 << (2 * i);
			m_calcAvgRT[i].Create(
				rtSize,
				rtSize,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
		}
		//最終的にトーンマップで使用する平均輝度を書き込むレンダリングターゲットを作成。
		for (auto& rt : m_avgRT) {
			rt.Create(
				1,
				1,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
		}

	}

	void CTonemap::InitShader()
	{
		m_vsShader.LoadVS(SHADER_FILEPATH, "VSMain");
		m_psCalcLuminanceLogAverage.LoadPS(SHADER_FILEPATH, "PSCalcLuminanceLogAverage");
		m_psCalcLuminanceAverage.LoadPS(SHADER_FILEPATH, "PSCalcLuminanceAverage");
		m_psCalcLuminanceExpAverage.LoadPS(SHADER_FILEPATH, "PSCalcLuminanceExpAverage");
		m_psCalcAdaptedLuminance.LoadPS(SHADER_FILEPATH, "PSCalcAdaptedLuminance");
		m_psCalcAdaptedLuminanceFirst.LoadPS(SHADER_FILEPATH, "PSCalcAdaptedLuminanceFirst");
		m_psFinal.LoadPS(SHADER_FILEPATH, "PSFinal");
	}

	void CTonemap::InitPipelineState()
	{
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };		//配列と要素数。
		psoDesc.pRootSignature = m_rootSignature.Get();									//ルートシグネチャを設定。
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsShader.GetCompiledBlob());

		//輝度の対数平均を求めるパイプラインステート。
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceLogAverage.GetCompiledBlob());
		m_calcLumionanceLogAveragePipelineState.Init(psoDesc);
		//ダウンサンプリングのパイプラインステート。
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceAverage.GetCompiledBlob());
		m_calcLuminanceAveragePipelineState.Init(psoDesc);
		//輝度の指数平均を求めるパイプラインステート。
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceExpAverage.GetCompiledBlob());
		m_calcLuminanceExpAveragePipelineState.Init(psoDesc);
		//明暗順応のためのパイプラインステート。
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcAdaptedLuminance.GetCompiledBlob());
		m_calcAdaptedLuminancePipelineState.Init(psoDesc);
		//明暗順応のためのパイプラインステート。(シーンが切り替わった時用。)
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcAdaptedLuminanceFirst.GetCompiledBlob());
		m_calcAdaptedLuminanceFirstPipelineState.Init(psoDesc);
		//最終描画のパイプラインステート。
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psFinal.GetCompiledBlob());
		m_finalPipelineState.Init(psoDesc);
	}

	void CTonemap::CreateConstantBuffer()
	{
		m_cbTonemapCommon.Init(sizeof(m_tonemapParam), &m_tonemapParam);
		for (int i = 0; i < NUM_CALC_AVG_RT; i++)
		{
			m_cbCalcLuminanceLog[i].Init(sizeof(m_avSampleOffsets), m_avSampleOffsets);
		}
	}

	void CTonemap::CreateDescriptorHeap()
	{
		//平均輝度計算用のディスクリプタヒープの作成。
		{
			//配列番号。
			int curNo = NUM_CALC_AVG_RT - 1;
			//輝度の対数平均の計算。
			m_calcAvgDescriptorHeap[curNo].RegistConstantBuffer(0, m_cbCalcLuminanceLog[curNo]);
			m_calcAvgDescriptorHeap[curNo].RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());
			m_calcAvgDescriptorHeap[curNo].Commit();

			curNo--;
			//テクスチャのダウンサンプリング。
			while (curNo > 0) {
				m_calcAvgDescriptorHeap[curNo].RegistConstantBuffer(0, m_cbCalcLuminanceLog[curNo]);
				m_calcAvgDescriptorHeap[curNo].RegistShaderResource(0, m_calcAvgRT[curNo + 1].GetRenderTargetTexture());
				m_calcAvgDescriptorHeap[curNo].Commit();
				curNo--;
			}
			//輝度の平均指数の計算。
			m_calcAvgDescriptorHeap[curNo].RegistConstantBuffer(0, m_cbCalcLuminanceLog[curNo]);
			m_calcAvgDescriptorHeap[curNo].RegistShaderResource(0, m_calcAvgRT[curNo + 1].GetRenderTargetTexture());
			m_calcAvgDescriptorHeap[curNo].Commit();

		}//平均輝度計算用のディスクリプタヒープ終わり。

		//明暗順応の計算。
		for (int i = 0; i < 2; i++)
		{
			m_calcAdaptedLuminanceDescriptorHeap[i].RegistConstantBuffer(1, m_cbTonemapCommon);
			m_calcAdaptedLuminanceDescriptorHeap[i].RegistShaderResource(1, m_calcAvgRT[0].GetRenderTargetTexture());	//平均輝度。
			m_calcAdaptedLuminanceDescriptorHeap[i].RegistShaderResource(2, m_avgRT[1 ^ i].GetRenderTargetTexture());	//一フレーム前の平均輝度。
			m_calcAdaptedLuminanceDescriptorHeap[i].Commit();
		}

		//最終描画用のディスクリプタヒープ。
		for (int i = 0; i < 2; i++) {
			m_finalDescriptorHeap[i].RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());		//シーンテクスチャ。
			m_finalDescriptorHeap[i].RegistShaderResource(1, m_avgRT[i].GetRenderTargetTexture());
			m_finalDescriptorHeap[i].Commit();
		}
	}

	void CTonemap::CalcLuminanceAverage(RenderContext& rc, CPostEffect* postEffect)
	{
		if (!m_isEnable) {
			return;
		}
		//対数平均を求める。
		float tU, tV;
		tU = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetWidth());
		tV = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetHeight());

		int index = 0;
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				m_avSampleOffsets[index].x = x * tU;
				m_avSampleOffsets[index].y = y * tV;

				index++;
			}
		}

		int curRtNo = NUM_CALC_AVG_RT - 1;
		//輝度の平均対数を求める。
		{
			//レンダリングターゲットの利用可能待ち。
			rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
			m_cbCalcLuminanceLog[curRtNo].CopyToVRAM(m_avSampleOffsets);		//定数バッファの更新。
			rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);
			rc.SetPipelineState(m_calcLumionanceLogAveragePipelineState);
			rc.SetRenderTargetAndViewport(&m_calcAvgRT[curRtNo]);
			//ドローコール。
			postEffect->DrawFullScreenQuad(rc);
			//レンダリングターゲットの書き込み完了待ち。
			rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
		}
		curRtNo--;
		//ダウンサンプリングを行って平均を求める。
		{
			while (curRtNo > 0) {
				//定数バッファの値を計算。
				GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
				//レンダリングターゲットの利用可能待ち。
				rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
				m_cbCalcLuminanceLog[curRtNo].CopyToVRAM(m_avSampleOffsets);		//定数バッファの更新。
				rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);
				rc.SetPipelineState(m_calcLuminanceAveragePipelineState);
				rc.SetRenderTargetAndViewport(&m_calcAvgRT[curRtNo]);
				//ドローコール。
				postEffect->DrawFullScreenQuad(rc);
				//レンダリングターゲットの書き込み完了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);

				curRtNo--;
			}
		}
		//exp関数を用いて最終平均を求める。
		{
			//定数バッファの値を計算。
			GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
			//レンダリングターゲットの利用可能待ち。
			rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
			m_cbCalcLuminanceLog[curRtNo].CopyToVRAM(m_avSampleOffsets);		//定数バッファの更新。
			rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);
			rc.SetPipelineState(m_calcLuminanceExpAveragePipelineState);
			rc.SetRenderTargetAndViewport(&m_calcAvgRT[curRtNo]);
			//ドローコール。
			postEffect->DrawFullScreenQuad(rc);
			//レンダリングターゲットの書き込み完了待ち。
			rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
		}
		//明暗順応。
		{
			//シーンが切り替わって初回。
			m_currentAvgRT = 1 ^ m_currentAvgRT;

			//レンダリングターゲットの利用可能待ち。
			rc.WaitUntilToPossibleSetRenderTarget(m_avgRT[m_currentAvgRT]);
			rc.SetDescriptorHeap(m_calcAdaptedLuminanceDescriptorHeap[m_currentAvgRT]);
			if (m_isFirstWhenChangeScene == true)
			{
				rc.SetPipelineState(m_calcAdaptedLuminanceFirstPipelineState);
				//初回フラグを消す。
				m_isFirstWhenChangeScene = false;
			}
			else {
				rc.SetPipelineState(m_calcAdaptedLuminancePipelineState);
			}
			rc.SetRenderTargetAndViewport(&m_avgRT[m_currentAvgRT]);
			//ドローコール。
			postEffect->DrawFullScreenQuad(rc);
			//レンダリングターゲットの書き込み完了待ち。
			rc.WaitUntilFinishDrawingToRenderTarget(m_avgRT[m_currentAvgRT]);
		}

	}

	void CTonemap::Render(RenderContext& rc, CPostEffect* postEffect)
	{
		if (!m_isEnable)
		{
			return;
		}
		//トーンマップの共通定数バッファを更新。
		m_tonemapParam.deltaTime = GameTime().GetFrameDeltaTime();
		m_cbTonemapCommon.CopyToVRAM(m_tonemapParam);
		//平均輝度の計算。
		CalcLuminanceAverage(rc, postEffect);

		//トーンマップの最終描画。
		auto& mainRT = GraphicsEngine()->GetMainRenderTarget();
		//レンダリングターゲット利用可能待ち。
		rc.WaitUntilToPossibleSetRenderTarget(mainRT);
		rc.SetPipelineState(m_finalPipelineState);
		rc.SetDescriptorHeap(m_finalDescriptorHeap[m_currentAvgRT]);
		rc.SetRenderTargetAndViewport(&mainRT);
		//描画。
		postEffect->DrawFullScreenQuad(rc);
		//レンダリングターゲット書き込み完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
	}

}