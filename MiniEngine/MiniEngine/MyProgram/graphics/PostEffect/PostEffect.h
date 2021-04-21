#pragma once

#include "Bloom.h"

namespace Engine {
	/// <summary>
	/// ポストエフェクト。
	/// </summary>
	class CPostEffect : Noncopyable
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CPostEffect();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		CPostEffect();
		/// <summary>
		/// 開放。
		/// </summary>
		void Release();
		/// <summary>
		/// 作成。
		/// </summary>
		/// <param name="config">コンフィグ。</param>
		void Create(const SPostEffectConfig& config);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void Render(RenderContext& rc);
		/// <summary>
		/// フルスクリーン描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void DrawFullScreenQuad(RenderContext& rc);
		/// <summary>
		/// 最終書き込みのレンダリングターゲットを切り替える。
		/// </summary>


	};

}
