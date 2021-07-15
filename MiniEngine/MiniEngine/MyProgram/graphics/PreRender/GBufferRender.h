#pragma once

namespace Engine {
	//GBufferの定義。
	enum class EnGBuffer {
		enGBufferAlbed,			//アルベド。
		enGBufferNormal,		//法線。
		enGBufferWorldPos,		//ワールド座標。
		enGBufferSpecular,		//スペキュラ。
		enGBufferShadow,		//影マップ。
		enGBufferReflection,	//反射率。
		enGBufferNum,			//GBufferの数。
	};

	class CGBufferRender : Noncopyable
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
		/// <summary>
		/// ディファードレンダリング。
		/// </summary>
		void Render(RenderContext& rc);
	public:
		/// <summary>
		/// ディファードレンダリングへ登録する。
		/// </summary>
		void RegistDefferdModel(Model* caster) {
			m_defferdModelArray.push_back(caster);
		}
		/// <summary>
		/// ディファードレンダリング登録を解除する。
		/// </summary>
		void RemoveDefferdModel(Model* caster)
		{
			auto it = std::find(m_defferdModelArray.begin(), m_defferdModelArray.end(), caster);
			if (it != m_defferdModelArray.end()) {
				m_defferdModelArray.erase(it);
			}
		}
		/// <summary>
		/// ディファードモデル登録をクリア。
		/// </summary>
		/// <remarks>エンジン内で呼び出す。</remarks>
		void ClearDefferdModel()
		{
			m_defferdModelArray.clear();
		}
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
		std::vector<Model*> m_defferdModelArray;								//ディファードで描画するモデル。
	};	//////class CGBufferRender
}

