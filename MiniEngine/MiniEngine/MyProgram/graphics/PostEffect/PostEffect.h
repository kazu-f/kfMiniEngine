#pragma once

#include "graphics/Primitive.h"
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
		~CPostEffect();
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

	private:
		/// <summary>
		/// �v���~�e�B�u���������B
		/// </summary>
		void InitPrimitive();

	private:
		CBloom m_bloom;				//�u���[���B
		CPrimitive m_quadPrimitive;	//�l�p�`�v���~�e�B�u�B
		int m_primitiveVertexNum = 0;	//�v���~�e�B�u�̒��_�̐��B
	};

}
