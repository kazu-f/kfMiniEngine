#pragma once


namespace Engine {
	/// <summary>
	/// 指向性シャドウマップクラス。
	/// </summary>
	/// <remarks>
	/// カスケードシャドウやってみる。
	/// </remarks>

	class CShadowMap : Noncopyable
	{
	public:
		/// <summary>
		/// 初期化処理。
		/// </summary>
		void Init(SShadowMapConfig& cnfig);
		/// <summary>
		/// シャドウマップへのレンダリングを行う。
		/// </summary>
		void RenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// シャドウマップへのレンダリング完了待ち。
		/// </summary>
		void WaitEndRenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// シャドウマップを有効にする。
		/// </summary>
		void SetEnable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// シャドウマップを無効にする。
		/// </summary>
		void SetDisable()
		{
			m_isEnable = false;
		}
	public:
		/// <summary>
		/// キャスターへ登録する。
		/// </summary>
		void RegistShadowCaster(Model* caster) {
			m_shadowCasters.push_back(caster);
		}
		/// <summary>
		/// キャスター登録をクリア。
		/// </summary>
		/// <remarks>エンジン内で呼び出す。</remarks>
		void ClearShadowCaster()
		{
			m_shadowCasters.clear();
		}
		/// <summary>
		/// 定数バッファを取得。
		/// </summary>
		ConstantBuffer& GetShadowMapConstantBuffer()
		{
			return m_shadowCb;
		}
		/// <summary>
		/// シャドウマップのレンダリングターゲットのテクスチャを取得。
		/// </summary>
		/// <remarks>0～2までの値で取得。</remarks>
		Texture* GetShadowMapTexture(int i)
		{
			if (i < NUM_SHADOW_MAP) {
				return &m_shadowMaps[i].GetRenderTargetTexture();
			}
			return nullptr;
		}

	private:	//内部的な処理。
		/// <summary>
		/// ライトの座標を計算する。
		/// </summary>
		Vector3 CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition);

	private:		//構造体とか
		/// <summary>
		/// シャドウマップ用の定数バッファの情報。
		/// </summary>
		struct SShadowCb {
			Matrix mLVP[NUM_SHADOW_MAP];
			Vector4 texOffset[NUM_SHADOW_MAP];
			float depthOffset[NUM_SHADOW_MAP];
			float pading;
			float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
		};

		std::vector<Model*> m_shadowCasters;					//シャドウキャスターの配列。
		Matrix m_LVPMatrix[NUM_SHADOW_MAP];							//ライトビュープロジェクション行列。
		SShadowCb m_shadowCbEntity;
	private:
		ConstantBuffer m_shadowCb;								//定数バッファ。
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];				//シャドウマップのレンダーターゲット。
		Vector3 m_lightDirection = { 0.0f,-1.0f,0.0f };			//ライトの方向。
		float m_shadowAreas[NUM_SHADOW_MAP] = { 0.0f };			//影が落ちる範囲。
		float m_lightHeight = 100.0f;							//ライトの高さ。
		bool m_isEnable = true;									//シャドウマップが有効かどうか。
	};
}

