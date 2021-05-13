#pragma once

#include "tkFile/TkmFile.h"

namespace Engine {
	/// <summary>
	/// �}�e���A���B
	/// </summary>
	class IMaterial {
	public:
		/// <summary>
		/// tkm�t�@�C���̃}�e���A����񂩂珉��������B
		/// </summary>
		/// <param name="tkmMat">tkm�}�e���A��</param>
		void InitFromTkmMaterila(
			const TkmFile::SMaterial& tkmMat,
			const wchar_t* fxFilePath,
			const char* vsEntryPointFunc,
			const char* psEntryPointFunc);
		/// <summary>
		/// �����_�����O���J�n����Ƃ��ɌĂяo���֐��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="maxInstance">�C���X�^���X�̐�</param>
		virtual void BeginRender(RenderContext& rc, int maxInstance = 1);

		/// <summary>
		/// �A���x�h�}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetAlbedoMap()
		{
			return m_albedoMap;
		}
		/// <summary>
		/// �@���}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetNormalMap()
		{
			return m_normalMap;
		}
		/// <summary>
		/// �X�y�L�����}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetSpecularMap()
		{
			return m_specularMap;
		}
		/// <summary>
		/// �萔�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		ConstantBuffer& GetConstantBuffer()
		{
			return m_constantBuffer;
		}
	protected:
		/// <summary>
		/// �p�C�v���C���X�e�[�g�̏������B
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// �V�F�[�_�[�̏������B
		/// </summary>
		/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
		/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
		/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
		virtual void InitShaders(const wchar_t* fxFilePath) = 0;
		/// <summary>
		/// �e�N�X�`�����������B
		/// </summary>
		/// <param name="tkmMat"></param>
		void InitTexture(const TkmFile::SMaterial& tkmMat);

	protected:
		/// <summary>
		/// �}�e���A���p�����[�^�B
		/// </summary>
		struct SMaterialParam {
			int hasNormalMap;	//�@���}�b�v��ێ����Ă��邩�ǂ����̃t���O�B
			int hasSpecMap;		//�X�y�L�����}�b�v��ێ����Ă��邩�ǂ����̃t���O�B
		};
		Texture	m_albedoMap;	//�A���x�h�}�b�v�B
		Texture	m_normalMap;							//�@���}�b�v�B
		Texture	m_specularMap;							//�X�y�L�����}�b�v�B
		ConstantBuffer m_constantBuffer;				//�萔�o�b�t�@�B
		//RootSignature m_rootSignature;					//���[�g�V�O�l�`���B
		//PipelineState m_ModelPipelineState;				//���f���p�̃p�C�v���C���X�e�[�g�B
		//PipelineState m_ModelInstancingPipelineState;	//�C���X�^���V���O���f���p�̃p�C�v���C���X�e�[�g�B
		//PipelineState m_transModelPipelineState;		//���f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B
		//PipelineState m_ModelShadowPipelineState;		//�V���h�E�}�b�v�̃��f���p�̃p�C�v���C���X�e�[�g�B
		//PipelineState m_ModelShadowInstancingPipelineState;		//�V���h�E�}�b�v�̃C���X�^���X���f���p�B

		//Shader m_vsModel;								//���f���p�̒��_�V�F�[�_�[�B
		//Shader m_vsModelInstancing;						//�C���X�^���V���O���f���p�̒��_�V�F�[�_�[�B
		//Shader m_psModel;								//���f���p�̃s�N�Z���V�F�[�_�[�B
		//Shader m_vsModelShadowMap;						//�V���h�E�}�b�v�̃��f���p�̒��_�V�F�[�_�[�B
		//Shader m_vsModelShadowInstancing;				//�V���h�E�}�b�v�̃C���X�^���X���f���p�B
		//Shader m_psModelShadowMap;						//�V���h�E�}�b�v�̃��f���p�̃s�N�Z���V�F�[�_�[�B
		//Shader m_psTransModel;							//�������̃��f���p�̃s�N�Z���V�F�[�_�[�B
	};
	/// <summary>
	/// �X�L�������}�e���A���B
	/// </summary>
	class NonSkinMaterial : public IMaterial {
	private:
		/// <summary>
		/// �V�F�[�_�[�̏������B
		/// </summary>
		/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
		void InitShaders(const wchar_t* fxFilePath)override final
		{};
		/// <summary>
		/// �����_�����O���J�n����Ƃ��ɌĂяo���֐��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="maxInstance">�C���X�^���X�̐�</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;

	};
	/// <summary>
	/// �X�L������}�e���A���B
	/// </summary>
	class SkinMaterial :public IMaterial{
	private:
		/// <summary>
		/// �V�F�[�_�[�̏������B
		/// </summary>
		/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
		void InitShaders(const wchar_t* fxFilePath)override final
		{};
		/// <summary>
		/// �����_�����O���J�n����Ƃ��ɌĂяo���֐��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="maxInstance">�C���X�^���X�̐�</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;
	};
}