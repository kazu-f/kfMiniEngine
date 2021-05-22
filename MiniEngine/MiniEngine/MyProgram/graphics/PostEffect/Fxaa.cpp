#include "stdafx.h"
#include "Fxaa.h"

namespace Engine {

	namespace {
		const wchar_t* SHADER_FILE_PATH = L"Assets/shader/fxaa.fx";
		const char* VS_NAME = "VSMain";
		const char* PS_NAME = "PSMain";
	}

	void CFxaa::Init(const SPostEffectConfig& config)
	{
		m_isEnable = config.isFxaa;
		if (!m_isEnable) {
			ENGINE_LOG("アンチエイリアスを適用しない。");
			return;
		}
		//ルートシグネチャを作成。
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);
		//シェーダーの初期化。
		InitShader();
		//パイプラインステートの作成。
		InitPipelineState();
		//ディスクリプタヒープの作成。
		CreateDescriptorHeap();
	}
	void CFxaa::Render(RenderContext& rc)
	{
		if (!m_isEnable) {
			return;
		}
		//メインレンダリングターゲットを取得。
		auto& mainRT = GraphicsEngine()->GetMainRenderTarget();
		//レンダリングターゲット利用可能待ち。
		rc.WaitUntilToPossibleSetRenderTarget(mainRT);
		//パイプラインステート設定。
		rc.SetPipelineState(m_fxaaPipelineState);
		//レンダリングターゲットを設定。
		rc.SetRenderTargetAndViewport(&mainRT);

		//ディスクリプタヒープの設定。
		rc.SetDescriptorHeap(m_descriptorHeap);
		//ドローコール。
		rc.DrawIndexed(4);
	}
	void CFxaa::InitShader()
	{
		m_vsShader.LoadVS(SHADER_FILE_PATH, VS_NAME);
		m_psShader.LoadPS(SHADER_FILE_PATH, PS_NAME);
	}
	void CFxaa::InitPipelineState()
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
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsShader.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psShader.GetCompiledBlob());
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
		//アンチエイリアス用パイプラインステート。
		m_fxaaPipelineState.Init(psoDesc);
	}
	void CFxaa::CreateDescriptorHeap()
	{
		//アンチエイリアス用のディスクリプタヒープを作成。
		m_descriptorHeap.RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());
		m_descriptorHeap.Commit();
	}
}
