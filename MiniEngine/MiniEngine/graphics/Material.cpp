#include "stdafx.h"
#include "Material.h"
namespace Engine {
	//ルートシグネチャとパイプラインステート周りはカリカリカリ
	enum {
		enDescriptorHeap_CB,
		enDescriptorHeap_SRV,
		enNumDescriptorHeap
	};

	void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
	{
		if (tkmMat.albedoMap != nullptr) {
			m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize);
		}
		if (tkmMat.normalMap != nullptr) {
			m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize);
		}
		if (tkmMat.specularMap != nullptr) {
			m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize);
		}
	}
	void Material::InitFromTkmMaterila(
		const TkmFile::SMaterial& tkmMat,
		const wchar_t* fxFilePath,
		const char* vsEntryPointFunc,
		const char* psEntryPointFunc)
	{
		//テクスチャをロード。
		InitTexture(tkmMat);

		//定数バッファを作成。
		SMaterialParam matParam;
		matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
		matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
		m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

		//ルートシグネチャを初期化。
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);

		//シェーダーを初期化。
		InitShaders(fxFilePath, vsEntryPointFunc, psEntryPointFunc);

		//パイプラインステートを初期化。
		InitPipelineState();

	}
	void Material::InitPipelineState()
	{
		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = TRUE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 3;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
#ifdef SAMPE_16_02
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//法線出力用。	
		psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z値。
#else
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//法線出力用。	
		psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z値。
#endif
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;

		m_skinModelPipelineState.Init(psoDesc);

		//続いてスキンなしモデル用を作成。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
		m_nonSkinModelPipelineState.Init(psoDesc);

		//シャドウマップ用のパイプラインステートを作成。
		//スキンあり。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelShadowMap.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModelShadowMap.GetCompiledBlob());
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
		m_skinModelShadowPipelineState.Init(psoDesc);
		//スキンなし。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModelShadowMap.GetCompiledBlob());
		m_nonSkinModelShadowPipelineState.Init(psoDesc);

		//続いて半透明マテリアル用。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psTransModel.GetCompiledBlob());
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
		psoDesc.BlendState.IndependentBlendEnable = TRUE;
		psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;


		m_transSkinModelPipelineState.Init(psoDesc);

		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
		m_transNonSkinModelPipelineState.Init(psoDesc);

	}
	void Material::InitShaders(
		const wchar_t* fxFilePath,
		const char* vsEntryPointFunc,
		const char* psEntryPointFunc
	)
	{
		m_vsNonSkinModel.LoadVS(fxFilePath, vsEntryPointFunc);
		m_vsSkinModel.LoadVS(fxFilePath, vsEntryPointFunc);
		m_psModel.LoadPS(fxFilePath, "PSMain_RenderGBuffer");
		m_vsNonSkinModelShadowMap.LoadVS(fxFilePath, "VSMainNonSkinShadowMap");
		m_vsSkinModelShadowMap.LoadVS(fxFilePath, "VSMainSkinShadowMap");
		m_psModelShadowMap.LoadPS(fxFilePath, "PSMainShadowMap");
		m_psTransModel.LoadPS(fxFilePath, psEntryPointFunc);
	}
	void Material::BeginRender(RenderContext& rc, int hasSkin)
	{
		rc.SetRootSignature(m_rootSignature);

		const auto renderStep = rc.GetRenderStep();

		switch (renderStep)
		{
		case enRenderStep_Undef:
			break;
		case enRenderStep_CreateDirectionalShadowMap:
			//シャドウマップの描画。
			if (hasSkin) {
				rc.SetPipelineState(m_skinModelShadowPipelineState);
			}
			else {
				rc.SetPipelineState(m_nonSkinModelShadowPipelineState);
			}
			break;
		case enRenderStep_PreRender:
			if (hasSkin) {
				rc.SetPipelineState(m_skinModelPipelineState);
			}
			else {
				rc.SetPipelineState(m_nonSkinModelPipelineState);
			}
			break;
		case enRenderStep_ForwardRender:
			if (hasSkin) {
				rc.SetPipelineState(m_transSkinModelPipelineState);
			}
			else {
				rc.SetPipelineState(m_transNonSkinModelPipelineState);
			}
			break;
		default:
			break;
		}

	}
}