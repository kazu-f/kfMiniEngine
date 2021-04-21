#pragma once

#include "Bloom.h"

namespace Engine {
	/// <summary>
	/// �|�X�g�G�t�F�N�g�B
	/// </summary>
	class CPostEffect : Noncopyable
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CPostEffect();
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		CPostEffect();
		/// <summary>
		/// �J���B
		/// </summary>
		void Release();
		/// <summary>
		/// �쐬�B
		/// </summary>
		/// <param name="config">�R���t�B�O�B</param>
		void Create(const SPostEffectConfig& config);
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void Render(RenderContext& rc);
		/// <summary>
		/// �t���X�N���[���`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void DrawFullScreenQuad(RenderContext& rc);
		/// <summary>
		/// �ŏI�������݂̃����_�����O�^�[�Q�b�g��؂�ւ���B
		/// </summary>


	};

}
