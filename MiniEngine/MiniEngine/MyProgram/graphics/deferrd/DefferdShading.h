#pragma once

/*
	ディファードレンダリングを行うクラス。
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
		/// ディファードレンダリングを行う。
		/// </summary>
		void Render(RenderContext& rc);
		/// <summary>
		/// キューブマップを設定する。
		/// </summary>
		/// <param name="tex"></param>
		void SetCubeMapTexture(Texture& tex)
		{
			m_cubeMapTexture.InitFromD3DResource(tex.Get());
			//ディスクリプタヒープを作り直して大丈夫かぁ？
			m_defferd.InitDescriptorHeap(m_initData);
		}

	private:
		Sprite m_defferd;				//ディファードの書き込み先。
		SpriteInitData	m_initData;		//初期化情報。
		Texture m_cubeMapTexture;		//テクスチャ。
	};

}

