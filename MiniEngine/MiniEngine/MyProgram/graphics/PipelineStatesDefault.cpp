#include "stdafx.h"
#include "PipelineStatesDefault.h"

namespace Engine
{
	//シェーダーのファイルパスとエントリ関数名。
	const wchar_t* CPipelineStatesDefault::MODEL_SHADER_PATH = L"Assets/shader/model.fx";								//モデルシェーダーのファイルパス。
	const char* CPipelineStatesDefault::VS_MODEL_ENTRY_POINT = "VSMain";												//通常モデルの頂点シェーダー。
	const char* CPipelineStatesDefault::VS_MODELINSTANCING_ENTRY_POINT = "VSMainInstancing";							//インスタンシングモデルの頂点シェーダー。
	const char* CPipelineStatesDefault::VS_SKINMODEL_ENTRY_POINT = "VSMainSkin";										//スキンモデルの頂点シェーダー。
	const char* CPipelineStatesDefault::VS_SKINMODELINTANCING_ENTRY_POINT = "VSMainSkinInstancing";						//スキンありインスタンシングモデルの頂点シェーダー。
	const char* CPipelineStatesDefault::VS_MODEL_SHADOW_ENTRY_POINT = "VSMainNonSkinShadowMap";							//シャドウマップ用頂点シェーダー。
	const char* CPipelineStatesDefault::VS_MODEL_SHADOWINSTANCING_ENTRY_POINT = "VSMainNonSkinInstancingShadowMap";		//シャドウマップ・インスタンシングモデル用頂点シェーダー。
	const char* CPipelineStatesDefault::VS_SKINMODEL_SHADOW_ENTRY_POINT = "VSMainSkinShadowMap";						//シャドウマップ・スキンありモデル用頂点シェーダー。
	const char* CPipelineStatesDefault::VS_SKINMODEL_SHADOWINSTANCING_ENTRY_POINT = "VSMainSkinInstancingShadowMap";	//シャドウマップ・スキンありインスタンシングモデル用頂点シェーダー。
	const char* CPipelineStatesDefault::PS_GBUFFER_ENTRY_POINT = "PSMain_RenderGBuffer";								//G-Bfferに書き込むピクセルシェーダー。
	const char* CPipelineStatesDefault::PS_SHADOW_ENTRY_POINT = "PSMainShadowMap";										//シャドウマップに書き込むピクセルシェーダー。
	const char* CPipelineStatesDefault::PS_TRANS_ENTRY_POINT = "PSMain";												//フォワード用頂点シェーダー。

	const wchar_t* CPipelineStatesDefault::COPY_SHADER_PATH = L"Assets/shader/copy.fx";				//コピー用シェーダーのファイルパス。
	const char* CPipelineStatesDefault::VS_COPY_ENTRY_POINT = "VSMain";								//コピー用頂点シェーダー。
	const char* CPipelineStatesDefault::PS_COPY_ENTRY_POINT = "PSMain";								//コピー用ピクセルシェーダー。

	//モデル用の設定。
	RootSignature CPipelineStatesDefault::m_modelDrawRootSignature;			//モデル描画用のルートシグネチャ。
	Shader CPipelineStatesDefault::m_vsModel;								//モデル用の頂点シェーダー。
	Shader CPipelineStatesDefault::m_vsModelInstancing;						//インスタンシングモデル用の頂点シェーダー。
	Shader CPipelineStatesDefault::m_vsSkinModel;							//モデル用の頂点シェーダー。(スキンあり)
	Shader CPipelineStatesDefault::m_vsSkinModelInstancing;					//インスタンシングモデル用の頂点シェーダー。(スキンあり)
	Shader CPipelineStatesDefault::m_psModel;								//モデル用のピクセルシェーダー。(G-Buffer書き込み。)
	Shader CPipelineStatesDefault::m_vsModelShadowMap;						//シャドウマップのモデル用の頂点シェーダー。
	Shader CPipelineStatesDefault::m_vsModelShadowInstancing;				//シャドウマップのインスタンスモデル用。
	Shader CPipelineStatesDefault::m_vsSkinModelShadowMap;					//シャドウマップのモデル用の頂点シェーダー。(スキンあり)
	Shader CPipelineStatesDefault::m_vsSkinModelShadowInstancing;			//シャドウマップのインスタンスモデル用。(スキンあり)
	Shader CPipelineStatesDefault::m_psModelShadowMap;						//シャドウマップのモデル用のピクセルシェーダー。
	Shader CPipelineStatesDefault::m_psTransModel;							//半透明のモデル用のピクセルシェーダー。

	PipelineState CPipelineStatesDefault::m_ModelPipelineState;				//モデル用のパイプラインステート。
	PipelineState CPipelineStatesDefault::m_ModelInstancingPipelineState;	//インスタンシングモデル用のパイプラインステート。
	PipelineState CPipelineStatesDefault::m_SkinModelPipelineState;				//モデル用のパイプラインステート。(スキンあり)
	PipelineState CPipelineStatesDefault::m_SkinModelInstancingPipelineState;	//インスタンシングモデル用のパイプラインステート。(スキンあり)
	PipelineState CPipelineStatesDefault::m_transModelPipelineState;		//モデル用のパイプラインステート(半透明マテリアル)。
	PipelineState CPipelineStatesDefault::m_transSkinModelPipelineState;		//モデル用のパイプラインステート(半透明マテリアル)。
	PipelineState CPipelineStatesDefault::m_transModelInstancingPipelineState;		//インスタンシングモデル用のパイプラインステート(半透明マテリアル)。
	PipelineState CPipelineStatesDefault::m_transSkinModelInstancingPipelineState;		//インスタンシングモデル用のパイプラインステート(半透明マテリアル)(スキンあり)。
	PipelineState CPipelineStatesDefault::m_ModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。
	PipelineState CPipelineStatesDefault::m_ModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。
	PipelineState CPipelineStatesDefault::m_SkinModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。(スキンあり)
	PipelineState CPipelineStatesDefault::m_SkinModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。(スキンあり)


	//コピー描画用の設定。
	Shader CPipelineStatesDefault::m_vsCopy;								//コピー用頂点シェーダー。
	Shader CPipelineStatesDefault::m_psCopy;								//コピー用ピクセルシェーダー。

	RootSignature CPipelineStatesDefault::m_copyRootSignature;				//コピー用のルートシグネチャ。
	PipelineState CPipelineStatesDefault::m_copyPipelineState;				//コピー用のパイプラインステート。


	void CPipelineStatesDefault::Init()
	{
		InitShaders();
		InitRootSignature();
		InitModelPipelineState();
		InitCopyPipelineState();
	}

	void CPipelineStatesDefault::InitShaders()
	{
		//モデルの頂点シェーダー。
		m_vsModel.LoadVS(MODEL_SHADER_PATH, VS_MODEL_ENTRY_POINT);
		m_vsModelInstancing.LoadVS(MODEL_SHADER_PATH, VS_MODELINSTANCING_ENTRY_POINT);
		m_vsSkinModel.LoadVS(MODEL_SHADER_PATH, VS_SKINMODEL_ENTRY_POINT);
		m_vsSkinModelInstancing.LoadVS(MODEL_SHADER_PATH, VS_SKINMODELINTANCING_ENTRY_POINT);

		//GBufferに書き込むピクセルシェーダー。
		m_psModel.LoadPS(MODEL_SHADER_PATH, PS_GBUFFER_ENTRY_POINT);

		//シャドウマップ関連。
		m_vsModelShadowMap.LoadVS(MODEL_SHADER_PATH, VS_MODEL_SHADOW_ENTRY_POINT);
		m_vsModelShadowInstancing.LoadVS(MODEL_SHADER_PATH, VS_MODEL_SHADOWINSTANCING_ENTRY_POINT);
		m_vsSkinModelShadowMap.LoadVS(MODEL_SHADER_PATH, VS_SKINMODEL_SHADOW_ENTRY_POINT);
		m_vsSkinModelShadowInstancing.LoadVS(MODEL_SHADER_PATH, VS_SKINMODEL_SHADOWINSTANCING_ENTRY_POINT);
		m_psModelShadowMap.LoadPS(MODEL_SHADER_PATH, PS_SHADOW_ENTRY_POINT);

		//透過オブジェクト描画。
		m_psTransModel.LoadPS(MODEL_SHADER_PATH, PS_TRANS_ENTRY_POINT);

		//コピー用シェーダー。
		m_vsCopy.LoadVS(COPY_SHADER_PATH, VS_COPY_ENTRY_POINT);
		m_psCopy.LoadPS(COPY_SHADER_PATH, PS_COPY_ENTRY_POINT);
	}

	void CPipelineStatesDefault::InitRootSignature()
	{
		//ルートシグネチャを初期化。
		m_modelDrawRootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);
		//コピー描画用ルートシグネチャ。
		m_copyRootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER
		);
	}

	void CPipelineStatesDefault::InitModelPipelineState()
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
		psoDesc.pRootSignature = m_modelDrawRootSignature.Get();
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = TRUE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 5;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
#ifdef SAMPE_16_02
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//法線出力用。	
		psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z値。
#else
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//法線出力用。	
		psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z値。
		psoDesc.RTVFormats[3] = DXGI_FORMAT_R32_FLOAT;			//スペキュラ。
		psoDesc.RTVFormats[4] = DXGI_FORMAT_R32_FLOAT;			//影。
#endif
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;


		//モデルのパイプラインステートを作成。
		{
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModel.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());
			m_ModelPipelineState.Init(psoDesc);
		
			//続いてインスタンシングモデル用を作成。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelInstancing.GetCompiledBlob());
			m_ModelInstancingPipelineState.Init(psoDesc);
		
			//スキンあり。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
			m_SkinModelPipelineState.Init(psoDesc);

			//スキンありインスタンシングモデル用を作成。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelInstancing.GetCompiledBlob());
			m_SkinModelInstancingPipelineState.Init(psoDesc);
		}

		//シャドウマップ用のパイプラインステートを作成。
		{
			//通常描画。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelShadowMap.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModelShadowMap.GetCompiledBlob());
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
			m_ModelShadowPipelineState.Init(psoDesc);
			//インスタンシング描画。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelShadowInstancing.GetCompiledBlob());
			m_ModelShadowInstancingPipelineState.Init(psoDesc);

			//スキンあり。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelShadowMap.GetCompiledBlob());
			m_SkinModelShadowPipelineState.Init(psoDesc);
			//スキンありインスタンシング描画。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelShadowInstancing.GetCompiledBlob());
			m_SkinModelShadowInstancingPipelineState.Init(psoDesc);

		}

		//半透明マテリアル用。
		{
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModel.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psTransModel.GetCompiledBlob());
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
			psoDesc.BlendState.IndependentBlendEnable = TRUE;
			psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
			psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			//スキン無し。
			m_transModelPipelineState.Init(psoDesc);
			//インスタンシング。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelInstancing.GetCompiledBlob());
			m_transModelInstancingPipelineState.Init(psoDesc);

			//スキンあり。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
			m_transSkinModelPipelineState.Init(psoDesc);
			//インスタンシング。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelInstancing.GetCompiledBlob());
			m_transSkinModelInstancingPipelineState.Init(psoDesc);


		}
	}

	void CPipelineStatesDefault::InitCopyPipelineState()
	{

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };		//配列と要素数。
		psoDesc.pRootSignature = m_copyRootSignature.Get();									//ルートシグネチャを設定。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsCopy.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCopy.GetCompiledBlob());
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
		//コピー用パイプラインステート。
		m_copyPipelineState.Init(psoDesc);
	}

}