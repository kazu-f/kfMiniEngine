#pragma once

/// <summary>
/// グラフィックに関する設定。
/// </summary>

namespace Engine {

	/// <summary>
	/// シャドウマップのコンフィグ。
	/// </summary>

	static const int NUM_SHADOW_MAP = 3;		//シャドウマップの数。

	struct SShadowMapConfig
	{
		bool 	isEnable = true;				//影を落とす処理が有効かどうかのフラグ。
		int		shadowMapWidth = 2048;			//シャドウマップの幅。
		int		shadowMapHeight = 2048;			//シャドウマップの高さ。
		float	shadowAreas[NUM_SHADOW_MAP] = { 200.0f ,400.0f , 800.0f };			//影が落ちる範囲。
		float	lightHeight = 1000.0f;			//ライトの高さ。
		//float	depthOffset[NUM_SHADOW_MAP] = { 0.0001f , 0.0001f , 0.0002f };	//!<シャドウオフセット。この値を大きくすると、影が落ちにくくなる。
		//float	offsetTexelWorld = 0.05f;			//!<ブラーの時にフェッチするオフセット座標。ワールド空間の量。値が大きいほど大きくボケる。
	};

}