#pragma once
/*
*	FXAA
*/

namespace Engine {
	class PostEffect;
	/// <summary>
	/// FXAA
	/// �|�X�g�G�t�F�N�g�ɂ��A���`�G�C���A�X���s���B
	/// </summary>
	class CFxaa : Noncopyable
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="config">�R���t�B�O</param>
		void Init(const SPostEffectConfig& config);
		
		void Render(RenderContext& rc);
	private:
		/// <summary>
		/// �V�F�[�_�[�̏������B
		/// </summary>
		void InitShader();
		/// <summary>
		/// �p�C�v���C���X�e�[�g�̍쐬�B
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̍쐬�B
		/// </summary>
		void CreateDescriptorHeap();
	private:
		Shader m_vsShader;
		Shader m_psShader;
		RootSignature m_rootSignature;
		PipelineState m_fxaaPipelineState;
		DescriptorHeap m_descriptorHeap;
		bool m_isEnable = false;
	};//class Fxaa;

}///namespace Engine;

