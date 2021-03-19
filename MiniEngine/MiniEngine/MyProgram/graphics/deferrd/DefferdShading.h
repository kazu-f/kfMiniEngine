#pragma once

/*
	ディファードレンダリングを行うクラス。
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
		/// ディファードレンダリングを行う。
		/// </summary>
		void Render(RenderContext& rc);


	private:
		Sprite m_defferd;				//ディファードの書き込み先。
		SpriteInitData	m_initData;		//初期化情報。
	};

}

