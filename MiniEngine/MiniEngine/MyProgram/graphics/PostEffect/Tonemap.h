#pragma once

namespace Engine {


	class CTonemap : Noncopyable
	{
	public:
		void Init(const SPostEffectConfig& config);


	public:
		/// <summary>
		/// 平均輝度の計算。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void CalcLuminanceAverage(RenderContext& rc);
		/// <summary>
		/// トーンマップの実行。
		/// <para>トーンマップの処理の前にCalcLuminanceAverageを実行して平均輝度の計算を行う必要がある。</para>
		/// <para>平均輝度の計算はポストエフェクトの先頭で行う。</para>
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void Render(RenderContext& rc);
		/// <summary>
		/// トーンマップの処理をリセット。
		/// <para>輝度が大きく変わるシーン切り替えなどが発生する場合などに、</para>
		/// <para>明暗順応がおかしく見える場合があるため、</para>
		/// <para>そのような場合にリセットを行う。</para>
		/// </summary>
		void Reset()
		{
			m_isFirstWhenChangeScene = true;
		}
		/// <summary>
		/// 明るさを設定。
		/// </summary>
		/// <param name="lum">明るさ。この値を大きくすると明るくなる。</param>
		void SetLuminance(float lum)
		{
			m_tonemapParam.middleGray = lum;
		}
		/// <summary>
		/// トーンマップの有効無効を設定。
		/// </summary>
		void SetEnable(bool flag)
		{
			m_isEnable = flag;
		}
	private:
		/// <summary>
		/// レンダリングターゲットの作成。
		/// </summary>
		void CreateRenderTargets();
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		void InitShader();
		/// <summary>
		/// パイプラインステートの作成。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// 定数バッファの作成。
		/// </summary>
		void CreateConstantBuffer();
		/// <summary>
		/// ディスクリプタヒープの作成。
		/// </summary>
		void CreateDescriptorHeap();

	public:
		static const int MAX_SAMPLES = 16;
	private:
		struct STonemapParam {
			float deltaTime;
			float middleGray;
		};
		static const	int NUM_CALC_AVG_RT = 5;			//平均輝度計算に使うレンダリングターゲットの枚数。
		RenderTarget	m_calcAvgRT[NUM_CALC_AVG_RT];		//平均輝度計算用のレンダリングターゲット。
		RenderTarget	m_avgRT[2];							//平均輝度が格納されるレンダリングターゲット。
		int				m_currentAvgRT = 0;					//現在使っているレンダリングターゲットの番号。
		RootSignature	m_rootSignature;					//ルートシグネチャ。
		Shader			m_vsShader;							//頂点シェーダー。
		Shader			m_psCalcLuminanceLogAverage;		//輝度の対数平均を求めるピクセルシェーダー。
		Shader			m_psCalcLuminanceAverage;			//輝度の平均を求めるピクセルシェーダー。
		Shader			m_psCalcLuminanceExpAverage;		//輝度の指数平均を求めるピクセルシェーダー。
		Shader			m_psCalcAdaptedLuminance;			//明暗順応のピクセルシェーダー。
		Shader			m_psCalcAdaptedLuminanceFirst;		//明暗順応のピクセルシェーダー。(シーンが切り替わった時に使用される。)
		Shader			m_psFinal;							//最終合成シェーダー。
		PipelineState	m_calcLumionanceLogAveragePipelineState;		//輝度の対数平均を求めるパイプラインステート。
		PipelineState	m_calcLuminanceAveragePipelineState;			//輝度の平均を求めるパイプラインステート。
		PipelineState	m_calcLuminanceExpAveragePipelineState;			//輝度の指数平均を求めるパイプラインステート。
		PipelineState	m_calcAdaptedLuminancePipelineState;			//明暗順応のためのパイプラインステート。
		PipelineState	m_calcAdaptedLuminanceFirstPipelineState;		//明暗順応のためのパイプラインステート。(シーンが切り替わった時に使用される。)
		PipelineState	m_finalPipelineState;							//最終描画のパイプラインステート。
		DescriptorHeap	m_calcAvgDescriptorHeap[NUM_CALC_AVG_RT];		//平均輝度計算用のディスクリプタヒープ。
		DescriptorHeap	m_calcAdaptedLuminanceDescriptorHeap[2];		//明暗順応用のディスクリプタヒープ。
		DescriptorHeap	m_finalDescriptorHeap[2];						//最終描画のディスクリプタヒープ。
		bool			m_isFirstWhenChangeScene = true;	//シーンが切り替わって初回の描画かどうかのフラグ。
		Vector4			m_avSampleOffsets[MAX_SAMPLES];
		ConstantBuffer	m_cbCalcLuminanceLog[NUM_CALC_AVG_RT];				//輝度の平均対数を求めるための定数バッファ。
		ConstantBuffer	m_cbTonemapCommon;					//トーンマップ共通の定数バッファ。
		STonemapParam	m_tonemapParam;
		bool			m_isEnable = false;					//トーンマップ有効化フラグ。
	};//class CTonemap;

}//namespace Engine;

