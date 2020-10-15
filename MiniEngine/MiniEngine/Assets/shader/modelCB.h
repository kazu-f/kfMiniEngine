

//モデル用の定数バッファ
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

/*
*	ライト用の定数バッファ
*	tkLightManager.hのSLightParamと対応する。
*/
cbuffer lightCb : register(b1)
{
	float3 eyePos			: packoffset(c0);		//カメラの位置。
	int numDirectionLight	: packoffset(c0.w);		//ディレクションライトの数。
	float3 ambientLight		: packoffset(c1);		//アンビエントライト。
	int numPointLight		: packoffset(c1.w);		//ポイントライトの数。
	float4 screenParam		: packoffset(c2);		//スクリーンパラメータ(neae,far,screenW,screenH)
}

/*
*	マテリアルの定数バッファ
*	Material.hのSMaterialParamに対応する。
*/
cbuffer MaterialParamCb : register(b2) {
	int hasNormalMap;		//法線マップがある？
	int hasSpecularMap;		//スペキュラマップがある？
}

//シャドウマップの数
static const int NUM_SHADOW_MAP = 3;
/*
*	シャドウマップ用の定数バッファ。
*/
cbuffer ShadowCb : register(b3) {
	float4x4 mLVP[NUM_SHADOW_MAP];		//ライトビュープロジェクション行列。
	float4 texOffset[NUM_SHADOW_MAP];	//シャドウマップのサイズ。
	float depthOffset[NUM_SHADOW_MAP];	//深度オフセット。シャドウアクネ(チラつき)回避のためのバイアス。
										//値が大きいほど影が落ちにくくなる。
	float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
}