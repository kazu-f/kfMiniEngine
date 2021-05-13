#pragma once

namespace Engine {
	/// <summary>
	/// �悭�g���p�C�v���C���X�e�[�g���܂Ƃ߂�B
	/// </summary>
	class CPipelineStatesDefault
	{
	public:
		static void Init();
		static RootSignature m_modelDrawRootSignature;			//���f���`��p�̃��[�g�V�O�l�`���B
		static Shader m_vsModel;								//���f���p�̒��_�V�F�[�_�[�B
		static Shader m_vsModelInstancing;						//�C���X�^���V���O���f���p�̒��_�V�F�[�_�[�B
		static Shader m_vsSkinModel;							//���f���p�̒��_�V�F�[�_�[�B(�X�L������)
		static Shader m_vsSkinModelInstancing;					//�C���X�^���V���O���f���p�̒��_�V�F�[�_�[�B(�X�L������)
		static Shader m_psModel;								//���f���p�̃s�N�Z���V�F�[�_�[�B(G-Buffer�������݁B)
		static Shader m_vsModelShadowMap;						//�V���h�E�}�b�v�̃��f���p�̒��_�V�F�[�_�[�B
		static Shader m_vsModelShadowInstancing;				//�V���h�E�}�b�v�̃C���X�^���X���f���p�B
		static Shader m_vsSkinModelShadowMap;					//�V���h�E�}�b�v�̃��f���p�̒��_�V�F�[�_�[�B(�X�L������)
		static Shader m_vsSkinModelShadowInstancing;			//�V���h�E�}�b�v�̃C���X�^���X���f���p�B(�X�L������)
		static Shader m_psModelShadowMap;						//�V���h�E�}�b�v�̃��f���p�̃s�N�Z���V�F�[�_�[�B
		static Shader m_psTransModel;							//�������̃��f���p�̃s�N�Z���V�F�[�_�[�B

		static PipelineState m_ModelPipelineState;				//���f���p�̃p�C�v���C���X�e�[�g�B
		static PipelineState m_ModelInstancingPipelineState;	//�C���X�^���V���O���f���p�̃p�C�v���C���X�e�[�g�B
		static PipelineState m_SkinModelPipelineState;			//���f���p�̃p�C�v���C���X�e�[�g�B(�X�L������)
		static PipelineState m_SkinModelInstancingPipelineState;//�C���X�^���V���O���f���p�̃p�C�v���C���X�e�[�g�B(�X�L������)
		static PipelineState m_transModelPipelineState;			//���f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B
		static PipelineState m_transSkinModelPipelineState;			//���f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B(�X�L������)
		static PipelineState m_ModelShadowPipelineState;		//�V���h�E�}�b�v�̃��f���p�̃p�C�v���C���X�e�[�g�B
		static PipelineState m_ModelShadowInstancingPipelineState;		//�V���h�E�}�b�v�̃C���X�^���X���f���p�B
		static PipelineState m_SkinModelShadowPipelineState;		//�V���h�E�}�b�v�̃��f���p�̃p�C�v���C���X�e�[�g�B(�X�L������)
		static PipelineState m_SkinModelShadowInstancingPipelineState;		//�V���h�E�}�b�v�̃C���X�^���X���f���p�B(�X�L������)

	private:
		//�V�F�[�_�[�̏������B
		static void InitShaders();
		//���[�g�V�O�l�`���̏������B
		static void InitRootSignature();
		//�p�C�v���C���X�e�[�g�������B
		static void InitPipelineState();

	};///class CPipelineStatesDefault;

}///namespace Engine;
