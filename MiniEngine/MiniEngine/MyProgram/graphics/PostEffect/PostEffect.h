#pragma once

#include "graphics/Primitive.h"
#include "Tonemap.h"
#include "Bloom.h"
#include "Fxaa.h"

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
		~CPostEffect();
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

	private:
		/// <summary>
		/// プリミティブを初期化。
		/// </summary>
		void InitPrimitive();

	private:
		CTonemap m_tonemap;
		CBloom m_bloom;				//ブルーム。
		CFxaa m_fxaa;				//アンチエイリアス。
		CPrimitive m_quadPrimitive;	//四角形プリミティブ。
		int m_primitiveVertexNum = 0;	//プリミティブの頂点の数。
	};

}
