#pragma once
#include "graphics/Primitive.h"

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

		void Init();
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
			//ディスクリプタヒープを作り直す。
			CreateDescriptorHeap();
		}

	private:
		/// <summary>
		/// 四角形プリミティブの作成。
		/// </summary>
		void InitPrimitive();
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		void InitShader();
		/// <summary>
		/// パイプラインステートの初期化。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// 定数バッファの初期化。
		/// </summary>
		void InitConstantBuffer();
		/// <summary>
		/// ディスクリプタヒープの作成。
		/// </summary>
		void CreateDescriptorHeap();

	private:
		//ディファードの定数バッファ構造体。
		struct SDefferdCB
		{
			Matrix mViewProjInv;					//ビュープロジェクション行列の逆行列。
		};

		CPrimitive m_primitive;						//四角形プリミティブ。
		Shader m_vsDefferd;							//ディファードの頂点シェーダー。
		Shader m_psDefferd;							//ディファードのピクセルシェーダー。
		PipelineState m_defferdPipelineState;		//パイプラインステート。
		DescriptorHeap m_defferdDescriptorHeap;		//ディスクリプタヒープ。
		ConstantBuffer m_constantBuffer;			//定数バッファ。
		Texture m_cubeMapTexture;		//テクスチャ。
	};

}

