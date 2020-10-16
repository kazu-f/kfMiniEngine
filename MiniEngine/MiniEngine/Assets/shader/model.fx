/*
*		モデルシェーダー。
*/

#include "modelCB.h"
#include "modelStruct.h"

static const float PI = 3.14159265358979323846;

//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
//ボーン行列。
StructuredBuffer<float4x4> boneMatrix : register(t3);
//ディレクションライト。
StructuredBuffer<SDirectionalLight> directionalLight : register(t4);

Texture2D<float4> shadowMap_0 : register(t5);		//シャドウマップ。
Texture2D<float4> shadowMap_1 : register(t6);		//シャドウマップ。
Texture2D<float4> shadowMap_2 : register(t7);		//シャドウマップ。
//サンプラステート。
sampler g_sampler : register(s0);

//ベックマン分布?	
/*
	m			(面の粗さ)
	t = N・H	(法線・ハーフベクトル)
	M = m^2		T = (N・H)^2
*/
float Beckmann(float m, float t)
{
	float M = m * m;
	float T = t * t;
	return exp((T - 1) / (M * T)) / (M * M * T);
}

//フレネル項?の近似式らしい？
float specFresnel(float f0, float u)
{
	return f0 + (1 - f0) * pow(1 - u, 5);
}

/*
*	BRDF(双方向反射率分布関数)
*	スペキュラの計算に使ってる。？
*	Lがライト
*	Vが視点	
*	Nが法線
*/
float BRDF(float3 L, float3 V, float3 N)
{
	float microfacet = 0.3f;		//マイクロファセット
									//表面の凸凹具合を表す的な？
									//面の粗さとかを調整するパラメータらしい？
	float f0 = 0.5f;				//謎の数字。
									//垂直入射時のフレネル反射係数???
	
	float3 H = normalize(L + V);		//ライト+視点のハーフベクトル。

	//色々内積取ってる。
	float NdotH = dot(N, H);		//法線とハーフベクトル
	float VdotH = dot(V, H);		//視点とハーフベクトル
	float NdotL = dot(N, L);		//法線とライト
	float NdotV = dot(N, V);		//法線と視点

	float D = Beckmann(microfacet, NdotH);	//微小面分布関数。
	float F = specFresnel(f0, VdotH);		//フレネル項の近似式。

	float t = 2.0 * NdotH / VdotH;			//計算の共通項を取っておく感じ。
	float G = max( 0.0f, min(1.0f,min(t * NdotV, t * NdotL)) );		//幾何学的減衰係数？
																	//最大値を1.0、最小値を0.0の間で
																	//値の小さいほうを取る

	float m = PI * NdotV * NdotL;			//クックトランス反射モデルの分母になる値？	

	/*
	*	F:フレネル項
	*	D:微小面分布
	*	G:幾何学的減衰係数
	*/
	return max(F * D * G / m, 0.0f);
}

/*
*	フレネル式？
*	なんか少し違う？1.0の部分が変数になってる？
*/
float SchlickFresnel(float u, float f0, float f90)
{
	//f90が1.0ならフレネル式?。
	return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

/*
*	正規化Disneyモデル拡散反射。
*	albedColor	:	ライトの強さか？
*	N			:	法線の正規化ベクトル
*	L			:	ライトへの正規化ベクトル
*	V			:	視点の正規化ベクトル
*	roughness	:	表面の粗さを表すパラメータらしい
*/
float3 NormalizedDisneyDiffuse(float3 baseColor, float3 N, float3 L, float3 V, float roughness)
{
	float3 H = normalize(L + V);		//ハーフベクトル。

	float energyBias = lerp(0.0f, 0.5f, roughness);				//なんか正規化のための数値？0.0～0.5の線形補完
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);	//同じく？なんかメンドイ線形補完してる感じする。
	//内積する。(下限0.0～上限1.0)
	float LdotH = saturate(dot(L, H));		//ライトとハーフベクトル
	float NdotL = saturate(dot(N, L));		//法線とライト
	float NdotV = saturate(dot(N, V));		//法線と視点

	//謎数値　フレネル反射率?(0.0～1.0)
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * roughness;
	
	//これだったら普通にフレネル式使うんでいいんでは？
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);	//フレネル係数(法線・ライト)
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);	//フレネル係数(法線・視点)

	/*
	*	よくわからん係数 * ライトの強さ？ 
	*	* フレネル係数(N・L) * フレネル係数(N・V) / PI
	*/
	return (energyFactor * baseColor * FL * FV) / PI;
}

//スキン行列を計算する関数。
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
		w += skinVert.Weights[i];
	}

	skinning += boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	return skinning;
}

/*
*	影が落ちているかを計算する。
*/
float CalcShadowPercent(Texture2D<float4> tex, float2 uv, float2 offset, float depth, float dOffset)
{
	float shadow_val = tex.Sample(g_sampler, uv).r;
	if (depth > shadow_val + dOffset) {
		return 1.0f;
	}
	return 0.0f;
}

/*
*	カスケードインデックスの取得。
*/
int GetCascadeIndex(float zInView)
{
	[unroll]
	for (int i = 0; i < NUM_SHADOW_MAP; i++) {
		if (zInView < shadowAreaDepthInViewSpace[i]) {
			return i;
		}
	}
	return 0;
}

/*
*	カスケードシャドウの処理。
*/
float CalcShadow(float3 worldPos, float zInView)
{
	float shadow = 0.0f;
	if (isShadowReceiver) {
		//影を落とす。
		//使用するシャドウマップ番号の取得。
		int cascadeIndex = GetCascadeIndex(zInView);

		float4 posInLVP = mul(mLVP[cascadeIndex], float4(worldPos, 1.0f));
		posInLVP.xyz /= posInLVP.w;

		float depth = min(posInLVP.z, 1.0f);

		//uv座標変換。
		float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);
		float shadow_val = 1.0f;

		if (cascadeIndex == 0) {
			shadow = CalcShadowPercent(shadowMap_0, shadowMapUV, texOffset[cascadeIndex], depth, depthOffset.x);
		}
		if (cascadeIndex == 1) {
			shadow = CalcShadowPercent(shadowMap_1, shadowMapUV, texOffset[cascadeIndex], depth, depthOffset.y);
		}
		if (cascadeIndex == 2) {
			shadow = CalcShadowPercent(shadowMap_2, shadowMapUV, texOffset[cascadeIndex], depth, depthOffset.z);
		}
	}
	return shadow;
}

//頂点シェーダーのコア関数。
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if (hasSkin) {
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else {
		m = mWorld;
	}

	psIn.pos = mul(m, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	psIn.worldPos = psIn.pos.xyz;
	psIn.pos = mul(mView, psIn.pos);						//ワールド座標系からカメラ座標系に変換。
	psIn.pos = mul(mProj, psIn.pos);						//カメラ座標系からスクリーン座標系に変換。
	psIn.normal = normalize(mul(m, vsIn.normal));		//法線をワールド座標系に変換。
	psIn.Tangent = normalize(mul(m, vsIn.Tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント。
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
	return VSMainCore(vsIn, false);
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
SPSIn VSMainSkin(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}

//物理ベースライティングのピクセルシェーダー。
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//法線の計算。
	float3 normal;
	if (hasNormalMap) {
		//法線マップから法線を引っ張ってくる。
		float3 binSpaceNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;	//-1.0f～1.0fに調整。
		normal = psIn.Tangent * binSpaceNormal.x		//接線(法線に対して右？)
			+ psIn.biNormal * binSpaceNormal.y			//従法線(法線に対して上)
			+ psIn.normal * binSpaceNormal.z;			//法線方向
	}
	else {
		normal = psIn.normal;
	}
	float3 lig = 0;		//ライト
	float3 toEye = normalize(eyePos - psIn.worldPos);		//点から視点までの正規化ベクトル
	float metaric = 0.0f;			//スペキュラ
	float4 albedoColor = g_texture.Sample(g_sampler, psIn.uv);	//テクスチャカラー。		
	if (hasSpecularMap) {
		//スペキュラマップがある。
		metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	}
	float roughness = 0.5f;			//拡散反射の面の粗さ。
	for (int ligNo = 0; ligNo < numDirectionLight; ligNo++)
	{
		float3 baseColor = max(dot(normal, -directionalLight[ligNo].direction), 0.0f) * directionalLight[ligNo].color;
		//DisneyModel拡散反射
		lig += NormalizedDisneyDiffuse(baseColor, normal, -directionalLight[ligNo].direction, toEye, roughness);
		//スペキュラ反射
		lig += BRDF(-directionalLight[ligNo].direction, toEye, normal) 
			* directionalLight[ligNo].color.xyz 
			* metaric * directionalLight[ligNo].color.w;
	}
	//環境光。
	lig += ambientLight;
	//シャドウ。
	float4 posInView = mul(mView, float4(psIn.worldPos,1.0f));
	float shadow = CalcShadow(psIn.worldPos, posInView.z);
	if (shadow == 1.0f)
	{
		lig *= 0.5f;
	}

	//最終的な色を決定する。
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}

/*
*	スキンなしモデルのシャドウマップ書き込み用の頂点シェーダー。
*/
SShadowMapPSIn VSMainNonSkinShadowMap(SShadowMapVSIn vsIn)
{
	SShadowMapPSIn psIn;
	psIn.pos = mul(mWorld, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);

	return psIn;
}
/*
*	スキンありモデルのシャドウマップ書き込み用の頂点シェーダー。
*/
SShadowMapPSIn VSMainSkinShadowMap(SShadowMapVSIn vsIn)
{
	SShadowMapPSIn psIn;
	float4x4 skinMatrix = CalcSkinMatrix(vsIn.skinVert);
	psIn.pos = mul(skinMatrix, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);

	return psIn;
}
/*
*	シャドウマップ書き込み用のピクセルシェーダー。
*/
float4 PSMainShadowMap(SShadowMapPSIn psIn) :SV_Target0
{
	return psIn.pos.z / psIn.pos.w;
}
