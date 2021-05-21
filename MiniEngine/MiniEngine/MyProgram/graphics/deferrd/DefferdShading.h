#pragma once

/*
	�f�B�t�@�[�h�����_�����O���s���N���X�B
*/

namespace Engine {

	class CGBufferRender;

	class CDefferdShading : Noncopyable
	{
	public:
		CDefferdShading() {};
		~CDefferdShading() {};

		void Init(CGBufferRender* gBuffer);
		/// <summary>
		/// �f�B�t�@�[�h�����_�����O���s���B
		/// </summary>
		void Render(RenderContext& rc);
		/// <summary>
		/// �L���[�u�}�b�v��ݒ肷��B
		/// </summary>
		/// <param name="tex"></param>
		void SetCubeMapTexture(Texture& tex)
		{
			m_cubeMapTexture.InitFromD3DResource(tex.Get());
			//�f�B�X�N���v�^�q�[�v����蒼���đ��v�����H
			m_defferd.InitDescriptorHeap(m_initData);
		}

	private:
		Sprite m_defferd;				//�f�B�t�@�[�h�̏������ݐ�B
		SpriteInitData	m_initData;		//���������B
		Texture m_cubeMapTexture;		//�e�N�X�`���B
	};

}

