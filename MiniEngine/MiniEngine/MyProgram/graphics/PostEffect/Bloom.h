#pragma once

namespace Engine {
	/// <summary>
	/// ブルーム。
	/// </summary>
	class CBloom : Noncopyable
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">グラフィック設定。</param>
		void Init(const SPostEffectConfig& config);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		/// <param name="postEffect">ポストエフェクト。</param>
		void Render(RenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// 重みを更新。
		/// </summary>
		void UpdateWeight(float dispersion);

	private:	//初期化等。
		/// <summary>
		/// レンダリングターゲットを初期化。
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// シェーダー初期化。
		/// </summary>
		void InitShaders();
		/// <summary>
		/// パイプラインステート初期化。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// ディスクリプタヒープの作成。
		/// </summary>
		void CreateDescriptorHeap();

	private:	//ブルームに使う処理。
		/// <summary>
		/// 輝度抽出。
		/// </summary>
		void SamplingLuminance(RenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// 輝度テクスチャにブラーを掛ける。
		/// </summary>
		void BlurLuminanceTexture(RenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// ボケ画像を合成。
		/// </summary>
		/// <param name="rc"></param>
		void CombineBlurImage(RenderContext& rc, CPostEffect* postEffect);
		/// <summary>
		/// メインレンダリングターゲットへの合成。
		/// </summary>
		void CombineMainRenderTarget(RenderContext& rc, CPostEffect* postEffect);

	private:
		static const int NUM_WEIGHTS = 8;			//重み付けする数。
		static const int NUM_DOWN_SAMPRING_RT = 6;		//ダウンサンプリングする数。(縦と横があるためダウンサンプリングしたい数 * 2)
		/// <summary>
		/// ブラー用のパラメータ。
		/// </summary>
		struct SBlurParam {
			Vector4 offset;
			float weights[NUM_WEIGHTS];
		};
		RootSignature m_rootSignature;		//ブルーム描画用のルートシグネチャ。
		//パイプラインステート
		PipelineState m_samplingLuminancePipelineState;			//輝度抽出用
		PipelineState m_xblurLuminancePipelineState;			//x方向に輝度をぼかすステップのパイプラインステート。
		PipelineState m_yblurLuminancePipelineState;			//y方向に輝度をぼかすステップのパイプラインステート。
		PipelineState m_combineBlurImagePipelineState;			//ボケ画像を合成するステップのパイプラインステート。
		PipelineState m_combineMainRenderTargetPipelineState;	//メインレンダリングターゲットへの合成ステップのパイプラインステート。
		//レンダリングターゲット。
		RenderTarget m_luminanceRT;								//輝度を抽出するためのレンダリングターゲット。
		RenderTarget m_combineRT;								//ぼかし合成用のレンダリングターゲット。
		RenderTarget m_downSamplingRT[NUM_DOWN_SAMPRING_RT];	//ダウンサンプリング用のレンダリングターゲット。
		//ブラー用のパラメータ。
		SBlurParam m_blurParam;									//ブラー用のパラメータ。
		//シェーダー。
		Shader m_vs;											//ただの頂点シェーダー。
		Shader m_psLuminance;									//輝度抽出パスのピクセルシェーダー。
		Shader m_vsXBlur;										//X方向ブラーパスの頂点シェーダー。
		Shader m_vsYBlur;										//Y方向ブラーパスの頂点シェーダー。
		Shader m_psBlur;										//X方向ブラー/Y方向ブラーパスのピクセルシェーダー。
		Shader m_psCombine;										//ボケ合成パスのピクセルシェーダー。
		//ブラー用の定数バッファ。
		ConstantBuffer m_blurParamCB[NUM_DOWN_SAMPRING_RT];		//ブラー用の定数バッファ。
		//ディスクリプタヒープ。
		DescriptorHeap m_sampleLuminanceDescriptorHeap;			//輝度抽出時に使用するディスクリプタヒープ。
		DescriptorHeap m_combineBlurDescriptorHeap;				//ボケ合成に使用するディスクリプタヒープ。
		DescriptorHeap m_downSampleDescriptorHeap[NUM_DOWN_SAMPRING_RT];	//ダウンサンプリング用のディスクリプタヒープ。
		DescriptorHeap m_combineMainRenderTargetDescriptorHeap;	//メインレンダリングターゲットへの合成に使用するディスクリプタヒープ。
		bool m_isEnable = false;
	};

}
