#pragma once

namespace Engine {

	class CFade
	{
	public:
		CFade();
		~CFade();
		/// <summary>
		/// 初期化処理。
		/// </summary>
		void Init();
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// フェードの描画を行う。
		/// </summary>
		void FadeRender(RenderContext& rc);
		/// <summary>
		/// フェードインを開始する。
		/// </summary>
		void StartFadeIn()
		{
			m_state = enState_fadeIn;
		}
		/// <summary>
		/// フェードアウトを開始する。
		/// </summary>
		void StartFadeOut()
		{
			m_state = enState_fadeOut;
		}
		/// <summary>
		/// フェード中かどうか。
		/// </summary>
		/// <returns></returns>
		bool IsFade()
		{
			return m_state != enState_idle;
		}
		/// <summary>
		/// フェードの時間を設定する。
		/// </summary>
		void SetFadeTime(const float fadeTime)
		{
			//0割り回避のために最低値を0.2秒にしておく。
			m_fadeTime = max(0.2f, fadeTime);
		}
		/// <summary>
		/// 現在のフェードの進行率を取得。
		/// </summary>
		const float GetFadeRate()
		{
			return 1.0f - m_currentAlpha;
		}

		static CFade* GetInstance()
		{
			return m_instance;
		}
	private:
		static CFade* m_instance;
	private:
		enum EnFadeState {
			enState_fadeIn,		//!<フェードイン中。
			enState_fadeOut,	//!<フェードアウト中。
			enState_idle		//!<アイドル中。
		};
		Sprite m_sprite;						//スプライト。
		EnFadeState m_state = enState_idle;		//状態。
		float m_currentAlpha = 1.0f;			//現在のα値。
		float m_fadeTime = 1.0f;				//フェードを行う時間(秒)。
	};

}
