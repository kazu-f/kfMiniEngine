#pragma once
/*
*	FXAA
*/

namespace Engine {
	class CPostEffect;
	/// <summary>
	/// FXAA
	/// ポストエフェクトによるアンチエイリアスを行う。
	/// </summary>
	class CFxaa : Noncopyable
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">コンフィグ</param>
		void Init(const SPostEffectConfig& config);
		/// <summary>
		/// アンチ実行。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		/// <param name="postEffect">ポストエフェクト。</param>
		void Render(RenderContext& rc, CPostEffect* postEffect);
	private:
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		void InitShader();
		/// <summary>
		/// パイプラインステートの作成。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// レンダリングターゲットの作成。
		/// </summary>
		void InitRenderTarget();
		/// <summary>
		/// ディスクリプタヒープの作成。
		/// </summary>
		void CreateDescriptorHeap();
	private:
		/// <summary>
		/// アンチエイリアスを掛ける。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		/// <param name="postEffect">ポストエフェクト。</param>
		void Fxaa(RenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// メインレンダリングターゲットに対して最終描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		/// <param name="postEffect">ポストエフェクト。</param>
		void FinalDraw(RenderContext& rc, CPostEffect* postEffect);
	private:
		RenderTarget m_fxaaRenderTarget;		//アンチを掛けたものを書き込むレンダリングターゲット。
		Shader m_vsShader;						//ただの頂点シェーダー。
		Shader m_psShader;						//アンチ用ピクセルシェーダー。
		RootSignature m_rootSignature;			//ルートシグネチャ。
		PipelineState m_fxaaPipelineState;		//アンチエイリアス用パイプラインステート。
		DescriptorHeap m_fxaaDescriptorHeap;	//アンチエイリアス用ディスクリプタヒープ。
		DescriptorHeap m_copyDescriptorHeap;	//メインレンダリングターゲットにコピーするためのディスクリプタヒープ。
		bool m_isEnable = false;
	};//class Fxaa;

}///namespace Engine;

