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
			ENGINE_LOG("�A���`�G�C���A�X��K�p���Ȃ��B");
			return;
		}
		//���[�g�V�O�l�`�����쐬�B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);
		//�V�F�[�_�[�̏������B
		InitShader();
		//�p�C�v���C���X�e�[�g�̍쐬�B
		InitPipelineState();
		//�f�B�X�N���v�^�q�[�v�̍쐬�B
		CreateDescriptorHeap();
	}
	void CFxaa::Render(RenderContext& rc)
	{
		if (!m_isEnable) {
			return;
		}
		//���C�������_�����O�^�[�Q�b�g���擾�B
		auto& mainRT = GraphicsEngine()->GetMainRenderTarget();
		//�����_�����O�^�[�Q�b�g���p�\�҂��B
		rc.WaitUntilToPossibleSetRenderTarget(mainRT);
		//�p�C�v���C���X�e�[�g�ݒ�B
		rc.SetPipelineState(m_fxaaPipelineState);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rc.SetRenderTargetAndViewport(&mainRT);

		//�f�B�X�N���v�^�q�[�v�̐ݒ�B
		rc.SetDescriptorHeap(m_descriptorHeap);
		//�h���[�R�[���B
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
		//�p�C�v���C���X�e�[�g���쐬�B
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };		//�z��Ɨv�f���B
		psoDesc.pRootSignature = m_rootSignature.Get();									//���[�g�V�O�l�`����ݒ�B
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
		//�A���`�G�C���A�X�p�p�C�v���C���X�e�[�g�B
		m_fxaaPipelineState.Init(psoDesc);
	}
	void CFxaa::CreateDescriptorHeap()
	{
		//�A���`�G�C���A�X�p�̃f�B�X�N���v�^�q�[�v���쐬�B
		m_descriptorHeap.RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());
		m_descriptorHeap.Commit();
	}
}
