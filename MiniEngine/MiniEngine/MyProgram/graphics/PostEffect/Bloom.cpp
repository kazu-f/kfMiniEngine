#include "stdafx.h"
#include "Bloom.h"

namespace Engine {
	namespace {
		//頂点データ。
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
		//シェーダーデータ。
		struct SShaderData {
			const wchar_t* filePath = nullptr;
			const char* funcName = nullptr;
		};
		
		const float BLOOM_WHEIGHT = 25.0f;

		const SShaderData VS = { L"Assets/shader/bloom.fx","VSMain" };		//汎用頂点シェーダー。
		const SShaderData PSLuminance = { L"Assets/shader/bloom.fx","PSSamplingLuminance" };	//輝度抽出用ピクセルシェーダー。
		const SShaderData VSXBlur = { L"Assets/shader/bloom.fx","VSXBlur" };		//Xブラー用頂点シェーダー。
		const SShaderData VSYBlur = { L"Assets/shader/bloom.fx","VSYBlur" };		//Yブラー用頂点シェーダー。
		const SShaderData PSBlur = { L"Assets/shader/bloom.fx","PSBlur" };			//ブラー用頂点シェーダー。
		const SShaderData PSCombine = { L"Assets/shader/bloom.fx","PSCombine" };	//ボケ合成用のピクセルシェーダー。

	}

	void CBloom::Init(const SPostEffectConfig& config)
	{
		if (!config.isBloom) {
			ENGINE_LOG("ブルームを適用しない。");
			return;
		}
		//ルートシグネチャを作成。
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER
		);

		//シェーダー初期化。
		InitShaders();
		//レンダリングターゲットを初期化。
		InitRenderTargets();
		//パイプラインステートを初期化。
		InitPipelineState();

		for (auto& cb : m_blurParamCB) {
			//定数バッファを初期化。
			cb.Init(sizeof(m_blurParam), nullptr);
		}
		//ディスクリプタヒープを作成。
		CreateDescriptorHeap();

		m_isEnable = true;
	}
	void CBloom::InitRenderTargets()
	{
		int w = GraphicsEngine()->GetFrameBufferWidth();
		int h = GraphicsEngine()->GetFrameBufferHeight();
		//輝度抽出用のレンダリングターゲットを作成。
		bool result = m_luminanceRT.Create(w, h, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
		ENGINE_ASSERT(result, "輝度抽出用のレンダリングターゲットの作成に失敗。");

		//ダウンサンプリング用のレンダリングターゲットを作成する。
		for (int i = 0; i < NUM_DOWN_SAMPRING_RT / 2; i++) {
			int shift = i + 1;		//シフト演算に使う数。
			int baseIndex = i * 2;	//レンダリングターゲットの開始位置。
			//横ブラー用
			result = m_downSamplingRT[baseIndex].Create(
				w >> shift,
				h >> (shift - 1),
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
			ENGINE_ASSERT(result, "横ブラー用のレンダリングターゲットの作成に失敗。");


			//縦ブラー用。
			result = m_downSamplingRT[baseIndex + 1].Create(
				w >> shift,
				h >> shift,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
			ENGINE_ASSERT(result, "縦ブラー用のレンダリングターゲットの作成に失敗。");
		}
		//ボケ画像合成用のレンダリングターゲット作成。
		result = m_combineRT.Create(w >> 2, h >> 2, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
		ENGINE_ASSERT(result,"ボケ画像合成用のレンダリングターゲットの作成に失敗。")
	}
	void CBloom::InitShaders()
	{
		//シェーダーをロード。
		m_vs.LoadVS(VS.filePath, VS.funcName);
		m_psLuminance.LoadPS(PSLuminance.filePath, PSLuminance.funcName);
		m_vsXBlur.LoadVS(VSXBlur.filePath, VSXBlur.funcName);
		m_vsYBlur.LoadVS(VSYBlur.filePath, VSYBlur.funcName);
		m_psBlur.LoadPS(PSBlur.filePath, PSBlur.funcName);
		m_psCombine.LoadPS(PSCombine.filePath, PSCombine.funcName);
	}
	void CBloom::InitPipelineState()
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
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psLuminance.GetCompiledBlob());
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
		//輝度抽出用パイプラインステート。
		m_samplingLuminancePipelineState.Init(psoDesc);

		//Xブラー用のパイプラインステート。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsXBlur.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psBlur.GetCompiledBlob());
		m_xblurLuminancePipelineState.Init(psoDesc);

		//Yブラー用のパイプラインステート。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsYBlur.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psBlur.GetCompiledBlob());
		m_yblurLuminancePipelineState.Init(psoDesc);

		//ボケ画像合成用のパイプラインステート。
		psoDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCombine.GetCompiledBlob());
		m_combineBlurImagePipelineState.Init(psoDesc);

		//最終合成用のパイプラインステート。
		psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDefault::m_vsCopy.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDefault::m_psCopy.GetCompiledBlob());
		m_combineMainRenderTargetPipelineState.Init(psoDesc);
	}
	void CBloom::CreateDescriptorHeap()
	{
		//輝度抽出用のディスクリプタヒープを作成。
		m_sampleLuminanceDescriptorHeap.RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());
		m_sampleLuminanceDescriptorHeap.Commit();
		
		//ボケ合成用のディスクリプタヒープを作成。
		for (int i = 0; i < NUM_DOWN_SAMPRING_RT / 2; i++) {
			int index = i * 2 + 1;
			m_combineBlurDescriptorHeap.RegistShaderResource(i, m_downSamplingRT[index].GetRenderTargetTexture());
		}
		m_combineBlurDescriptorHeap.Commit();

		//輝度抽出テクスチャを設定する。
		RenderTarget* prevRt = &m_luminanceRT;

		int rtNo = 0;
		for (int i = 0; i < NUM_DOWN_SAMPRING_RT / 2; i++) {
			//Xブラー。
			{
				m_downSampleDescriptorHeap[rtNo].RegistShaderResource(0, prevRt->GetRenderTargetTexture());
				m_downSampleDescriptorHeap[rtNo].RegistConstantBuffer(0, m_blurParamCB[rtNo]);
				m_downSampleDescriptorHeap[rtNo].Commit();
			}
			//Xブラーを掛けたテクスチャをシェーダーリソースとして使う。
			prevRt = &m_downSamplingRT[rtNo];
			rtNo++;
			//Yブラー。
			{
				m_downSampleDescriptorHeap[rtNo].RegistShaderResource(0,prevRt->GetRenderTargetTexture());
				m_downSampleDescriptorHeap[rtNo].RegistConstantBuffer(0, m_blurParamCB[rtNo]);
				m_downSampleDescriptorHeap[rtNo].Commit();
			}
			//ダウンサンプリング後のテクスチャをシェーダーリソースとして使う。
			prevRt = &m_downSamplingRT[rtNo];
			rtNo++;
		}

		m_combineMainRenderTargetDescriptorHeap.RegistShaderResource(0, m_combineRT.GetRenderTargetTexture());
		m_combineMainRenderTargetDescriptorHeap.Commit();
	}
	void CBloom::UpdateWeight(float dispersion)
	{
		float total = 0;
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] = expf(-0.5f * static_cast<float>(i * i) / dispersion);
			total += 2.0f * m_blurParam.weights[i];
		}
		//規格化
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] /= total;
		}

	}
	void CBloom::SamplingLuminance(RenderContext& rc, CPostEffect* postEffect)
	{
		//メインレンダリングターゲットをテクスチャとして利用出来るようになるまで待機。
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRT);
		//パイプラインステートを設定。
		rc.SetPipelineState(m_samplingLuminancePipelineState);
		//レンダリングターゲットを輝度抽出用に切り替える。
		rc.SetRenderTargetAndViewport(&m_luminanceRT);
		//クリアカラーを設定。
		const float clearColor[] = { 0.0f,0.0f,0.0f,1.0f };
		rc.ClearRenderTargetView(m_luminanceRT.GetRTVCpuDescriptorHandle(), clearColor);
		//シェーダーリソースビューと定数バッファをセットする。
		rc.SetDescriptorHeap(m_sampleLuminanceDescriptorHeap);
		//輝度抽出テクスチャを描画する。
		postEffect->DrawFullScreenQuad(rc);
		//描画完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRT);
	}
	void CBloom::BlurLuminanceTexture(RenderContext& rc, CPostEffect* postEffect)
	{
		//テクスチャ。
		RenderTarget* prevRt = &m_luminanceRT;
		//レンダリングターゲットのインデックス。
		int rtIndex = 0;
		//ブラーを掛けていく。
		for (int i = 0; i < NUM_DOWN_SAMPRING_RT / 2; i++){
			//Xブラー。
			{
				m_blurParam.offset.x = 16.0f / prevRt->GetWidth();
				m_blurParam.offset.y = 0.0f;
				//定数バッファの値を更新。
				m_blurParamCB[rtIndex].CopyToVRAM(&m_blurParam);

				//メインレンダリングターゲットとして利用可能待ち。
				rc.WaitUntilToPossibleSetRenderTarget(m_downSamplingRT[rtIndex]);
				//Xブラー用のパイプラインステートを設定。
				rc.SetPipelineState(m_xblurLuminancePipelineState);
				//レンダリングターゲットを設定。
				rc.SetRenderTargetAndViewport(&m_downSamplingRT[rtIndex]);
				//シェーダーリソースビューと定数バッファのセット。
				rc.SetDescriptorHeap(m_downSampleDescriptorHeap[rtIndex]);
				//ドローコール。
				postEffect->DrawFullScreenQuad(rc);
				//描画完了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_downSamplingRT[rtIndex]);
			}
			//Yブラー用にテクスチャ変更。
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;

			//Yブラー。
			{
				m_blurParam.offset.x = 0.0f;
				m_blurParam.offset.y = 16.0f/ prevRt->GetWidth();
				//定数バッファの値を更新。
				m_blurParamCB[rtIndex].CopyToVRAM(&m_blurParam);

				//メインレンダリングターゲットとして利用可能待ち。
				rc.WaitUntilToPossibleSetRenderTarget(m_downSamplingRT[rtIndex]);
				//Yブラー用のパイプラインステートを設定。
				rc.SetPipelineState(m_yblurLuminancePipelineState);
				//レンダリングターゲットを設定。
				rc.SetRenderTargetAndViewport(&m_downSamplingRT[rtIndex]);
				//シェーダーリソースビューと定数バッファのセット。
				rc.SetDescriptorHeap(m_downSampleDescriptorHeap[rtIndex]);
				//ドローコール。
				postEffect->DrawFullScreenQuad(rc);
				//描画完了待ち。
				rc.WaitUntilFinishDrawingToRenderTarget(m_downSamplingRT[rtIndex]);
			}
			//ダウンサンプリングした後のテクスチャに変更。
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;
		}

	}
	void CBloom::CombineBlurImage(RenderContext& rc, CPostEffect* postEffect)
	{
		//メインレンダリングターゲットとして利用可能待ち。
		rc.WaitUntilToPossibleSetRenderTarget(m_combineRT);
		//ボケ合成用のパイプラインステートをセット。
		rc.SetPipelineState(m_combineBlurImagePipelineState);
		//レンダリングターゲットを設定。
		rc.SetRenderTargetAndViewport(&m_combineRT);

		//ディスクリプタヒープをセット。
		rc.SetDescriptorHeap(m_combineBlurDescriptorHeap);
		//ドローコール。
		postEffect->DrawFullScreenQuad(rc);
		//描画完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(m_combineRT);
	}
	void CBloom::CombineMainRenderTarget(RenderContext& rc, CPostEffect* postEffect)
	{
		//メインレンダリングターゲットを取得。
		auto& mainRT = GraphicsEngine()->GetMainRenderTarget();
		//レンダリングターゲット利用可能待ち。
		rc.WaitUntilToPossibleSetRenderTarget(mainRT);
		//メインレンダリングターゲットに合成するためのパイプラインステートをセット。
		rc.SetPipelineState(m_combineMainRenderTargetPipelineState);
		//レンダリングターゲットを設定。
		rc.SetRenderTargetAndViewport(&mainRT);

		//ディスクリプタヒープの設定。
		rc.SetDescriptorHeap(m_combineMainRenderTargetDescriptorHeap);
		//ドローコール。
		postEffect->DrawFullScreenQuad(rc);
		//レンダリングターゲット書き込み完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
	}
	void CBloom::Render(RenderContext& rc, CPostEffect* postEffect)
	{
		if (!m_isEnable) {
			//初期化されていない。
			return;
		}

		//重みを更新。
		UpdateWeight(BLOOM_WHEIGHT);
		//ルートシグネチャを設定。
		rc.SetRootSignature(m_rootSignature);
		//輝度を抽出。
		SamplingLuminance(rc, postEffect);
		//輝度をぼかす。
		BlurLuminanceTexture(rc, postEffect);
		//ぼかした画像を合成。
		CombineBlurImage(rc, postEffect);
		//メインレンダリングターゲットに合成。
		CombineMainRenderTarget(rc, postEffect);
	}
}

