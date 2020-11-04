#pragma once

namespace Engine {
	//GBufferの定義。
	enum class EnGBuffer {
		enGBufferAlbed,			//アルベド。
		enGBufferNormal,		//法線。
		enGBufferTangent,		//接法線。
		enGBufferWorldPos,		//ワールド座標。
		enGBufferSpecular,		//スペキュラ。
		enGBufferShadow,		//影マップ。
		enGBufferNum,			//GBufferの数。
	};

	class CGBufferRender
	{
	public:
		CGBufferRender();
		~CGBufferRender();
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// GBufferを書き込み先に設定。
		/// </summary>
		void BeginRender(RenderContext& rc);
		/// <summary>
		/// 書き込み先を元に戻す。
		/// </summary>
		void EndRender(RenderContext& rc);
	public:		/////Get関数類
		/// <summary>
		/// レンダリングターゲットを取得。
		/// </summary>
		/// <param name="enGBuffer"></param>
		/// <returns></returns>
		RenderTarget& GetRenderTarget(EnGBuffer enGBuffer)
		{
			return m_GBuffer[static_cast<int>(enGBuffer)];
		}
		/// <summary>
		/// GBufferのテクスチャを取得。
		/// </summary>
		/// <param name="enGBuffer"></param>
		/// <returns></returns>
		Texture& GetGBufferTexture(EnGBuffer enGBuffer)
		{
			return m_GBuffer[static_cast<int>(enGBuffer)].GetRenderTargetTexture();
		}
	private:
		RenderTarget m_GBuffer[static_cast<int>(EnGBuffer::enGBufferNum)];		//GBuffer。
		//ConstantBuffer m_cb;				//定数バッファ。
	};	//////class CGBufferRender
}

