#include "stdafx.h"
#include "PipelineStatesDefault.h"

namespace Engine
{
	const wchar_t* CPipelineStatesDefault::MODEL_SHADER_PAHT = L"Assets/shader/model.fx";
	const char* CPipelineStatesDefault::VS_MODEL_ENTRY_POINT = "VSMain";
	const char* CPipelineStatesDefault::VS_MODELINSTANCING_ENTRY_POINT = "VSMainInstancing";
	const char* CPipelineStatesDefault::VS_SKINMODEL_ENTRY_POINT = "VSMainSkin";
	const char* CPipelineStatesDefault::VS_SKINMODELINTANCING_ENTRY_POINT = "VSMainSkinInstancing";
	const char* CPipelineStatesDefault::VS_MODEL_SHADOW_ENTRY_POINT = "VSMainNonSkinShadowMap";
	const char* CPipelineStatesDefault::VS_MODEL_SHADOWINSTANCING_ENTRY_POINT = "VSMainNonSkinInstancingShadowMap";
	const char* CPipelineStatesDefault::VS_SKINMODEL_SHADOW_ENTRY_POINT = "VSMainSkinShadowMap";
	const char* CPipelineStatesDefault::VS_SKINMODEL_SHADOWINSTANCING_ENTRY_POINT = "VSMainSkinInstancingShadowMap";
	const char* CPipelineStatesDefault::PS_GBUFFER_ENTRY_POINT = "PSMain_RenderGBuffer";
	const char* CPipelineStatesDefault::PS_SHADOW_ENTRY_POINT = "PSMainShadowMap";
	const char* CPipelineStatesDefault::PS_TRANS_ENTRY_POINT = "PSMain";

	RootSignature CPipelineStatesDefault::m_modelDrawRootSignature;			//���f���`��p�̃��[�g�V�O�l�`���B
	Shader CPipelineStatesDefault::m_vsModel;								//���f���p�̒��_�V�F�[�_�[�B
	Shader CPipelineStatesDefault::m_vsModelInstancing;						//�C���X�^���V���O���f���p�̒��_�V�F�[�_�[�B
	Shader CPipelineStatesDefault::m_vsSkinModel;							//���f���p�̒��_�V�F�[�_�[�B(�X�L������)
	Shader CPipelineStatesDefault::m_vsSkinModelInstancing;					//�C���X�^���V���O���f���p�̒��_�V�F�[�_�[�B(�X�L������)
	Shader CPipelineStatesDefault::m_psModel;								//���f���p�̃s�N�Z���V�F�[�_�[�B(G-Buffer�������݁B)
	Shader CPipelineStatesDefault::m_vsModelShadowMap;						//�V���h�E�}�b�v�̃��f���p�̒��_�V�F�[�_�[�B
	Shader CPipelineStatesDefault::m_vsModelShadowInstancing;				//�V���h�E�}�b�v�̃C���X�^���X���f���p�B
	Shader CPipelineStatesDefault::m_vsSkinModelShadowMap;					//�V���h�E�}�b�v�̃��f���p�̒��_�V�F�[�_�[�B(�X�L������)
	Shader CPipelineStatesDefault::m_vsSkinModelShadowInstancing;			//�V���h�E�}�b�v�̃C���X�^���X���f���p�B(�X�L������)
	Shader CPipelineStatesDefault::m_psModelShadowMap;						//�V���h�E�}�b�v�̃��f���p�̃s�N�Z���V�F�[�_�[�B
	Shader CPipelineStatesDefault::m_psTransModel;							//�������̃��f���p�̃s�N�Z���V�F�[�_�[�B

	PipelineState CPipelineStatesDefault::m_ModelPipelineState;				//���f���p�̃p�C�v���C���X�e�[�g�B
	PipelineState CPipelineStatesDefault::m_ModelInstancingPipelineState;	//�C���X�^���V���O���f���p�̃p�C�v���C���X�e�[�g�B
	PipelineState CPipelineStatesDefault::m_SkinModelPipelineState;				//���f���p�̃p�C�v���C���X�e�[�g�B(�X�L������)
	PipelineState CPipelineStatesDefault::m_SkinModelInstancingPipelineState;	//�C���X�^���V���O���f���p�̃p�C�v���C���X�e�[�g�B(�X�L������)
	PipelineState CPipelineStatesDefault::m_transModelPipelineState;		//���f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B
	PipelineState CPipelineStatesDefault::m_transSkinModelPipelineState;		//���f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B
	PipelineState CPipelineStatesDefault::m_ModelShadowPipelineState;		//�V���h�E�}�b�v�̃��f���p�̃p�C�v���C���X�e�[�g�B
	PipelineState CPipelineStatesDefault::m_ModelShadowInstancingPipelineState;		//�V���h�E�}�b�v�̃C���X�^���X���f���p�B
	PipelineState CPipelineStatesDefault::m_SkinModelShadowPipelineState;		//�V���h�E�}�b�v�̃��f���p�̃p�C�v���C���X�e�[�g�B(�X�L������)
	PipelineState CPipelineStatesDefault::m_SkinModelShadowInstancingPipelineState;		//�V���h�E�}�b�v�̃C���X�^���X���f���p�B(�X�L������)


	void CPipelineStatesDefault::Init()
	{
		InitShaders();
		InitRootSignature();
		InitPipelineState();
	}

	void CPipelineStatesDefault::InitShaders()
	{
		//���f���̒��_�V�F�[�_�[�B
		m_vsModel.LoadVS(MODEL_SHADER_PAHT, VS_MODEL_ENTRY_POINT);
		m_vsModelInstancing.LoadVS(MODEL_SHADER_PAHT, VS_MODELINSTANCING_ENTRY_POINT);
		m_vsSkinModel.LoadVS(MODEL_SHADER_PAHT, VS_SKINMODEL_ENTRY_POINT);
		m_vsSkinModelInstancing.LoadVS(MODEL_SHADER_PAHT, VS_SKINMODELINTANCING_ENTRY_POINT);

		//GBuffer�ɏ������ރs�N�Z���V�F�[�_�[�B
		m_psModel.LoadPS(MODEL_SHADER_PAHT, PS_GBUFFER_ENTRY_POINT);

		//�V���h�E�}�b�v�֘A�B
		m_vsModelShadowMap.LoadVS(MODEL_SHADER_PAHT, VS_MODEL_SHADOW_ENTRY_POINT);
		m_vsModelShadowInstancing.LoadVS(MODEL_SHADER_PAHT, VS_MODEL_SHADOWINSTANCING_ENTRY_POINT);
		m_vsSkinModelShadowMap.LoadVS(MODEL_SHADER_PAHT, VS_SKINMODEL_SHADOW_ENTRY_POINT);
		m_vsSkinModelShadowInstancing.LoadVS(MODEL_SHADER_PAHT, VS_SKINMODEL_SHADOWINSTANCING_ENTRY_POINT);
		m_psModelShadowMap.LoadPS(MODEL_SHADER_PAHT, PS_SHADOW_ENTRY_POINT);

		//���߃I�u�W�F�N�g�`��B
		m_psTransModel.LoadPS(MODEL_SHADER_PAHT, PS_TRANS_ENTRY_POINT);
	}

	void CPipelineStatesDefault::InitRootSignature()
	{
		//���[�g�V�O�l�`�����������B
		m_modelDrawRootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);
	}

	void CPipelineStatesDefault::InitPipelineState()
	{
		// ���_���C�A�E�g���`����B
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

		//�p�C�v���C���X�e�[�g���쐬�B
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
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//�A���x�h�J���[�o�͗p�B
#ifdef SAMPE_16_02
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//�@���o�͗p�B	
		psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z�l�B
#else
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//�@���o�͗p�B	
		psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z�l�B
		psoDesc.RTVFormats[3] = DXGI_FORMAT_R32_FLOAT;			//�X�y�L�����B
		psoDesc.RTVFormats[4] = DXGI_FORMAT_R32_FLOAT;			//�e�B
#endif
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;


		//���f���̃p�C�v���C���X�e�[�g���쐬�B
		{
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModel.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());
			m_ModelPipelineState.Init(psoDesc);
		
			//�����ăC���X�^���V���O���f���p���쐬�B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelInstancing.GetCompiledBlob());
			m_ModelInstancingPipelineState.Init(psoDesc);
		
			//�X�L������B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
			m_SkinModelPipelineState.Init(psoDesc);

			//�X�L������C���X�^���V���O���f���p���쐬�B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelInstancing.GetCompiledBlob());
			m_SkinModelInstancingPipelineState.Init(psoDesc);
		}

		//�V���h�E�}�b�v�p�̃p�C�v���C���X�e�[�g���쐬�B
		{
			//�ʏ�`��B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelShadowMap.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModelShadowMap.GetCompiledBlob());
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
			m_ModelShadowPipelineState.Init(psoDesc);
			//�C���X�^���V���O�`��B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModelShadowInstancing.GetCompiledBlob());
			m_ModelShadowInstancingPipelineState.Init(psoDesc);

			//�X�L������B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelShadowMap.GetCompiledBlob());
			m_SkinModelShadowPipelineState.Init(psoDesc);
			//�X�L������C���X�^���V���O�`��B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModelShadowInstancing.GetCompiledBlob());
			m_SkinModelShadowInstancingPipelineState.Init(psoDesc);

		}

		//�������}�e���A���p�B
		{
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsModel.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psTransModel.GetCompiledBlob());
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//�A���x�h�J���[�o�͗p�B
			psoDesc.BlendState.IndependentBlendEnable = TRUE;
			psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
			psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

			m_transModelPipelineState.Init(psoDesc);

			//�X�L������B
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());

			m_transSkinModelPipelineState.Init(psoDesc);
		}
	}

}