#pragma once

/*
	�f�B�t�@�[�h�����_�����O���s���N���X�B
*/

namespace Engine {

	class CGBufferRender;

	class CDefferdShading
	{
	public:
		CDefferdShading() {};
		~CDefferdShading() {};

		void Init(CGBufferRender* gBuffer);
		/// <summary>
		/// �f�B�t�@�[�h�����_�����O���s���B
		/// </summary>
		void Render(RenderContext& rc);


	private:
		Sprite m_defferd;				//�f�B�t�@�[�h�̏������ݐ�B
		SpriteInitData	m_initData;		//���������B
	};

}

