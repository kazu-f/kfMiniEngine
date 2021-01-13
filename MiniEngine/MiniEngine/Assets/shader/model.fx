/*
*		モデルシェーダー。
*/

#include "modelCB.h"
#include "modelStruct.h"
#include "PBR.h"

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

StructuredBuffer<float4x4> instancingDatas : register(t8);		//インスタンシング描画用のデータ。
//サンプラステート。
sampler g_sampler : register(s0);

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
	return -1;
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
		if (cascadeIndex == -1) {
			return shadow;
		}

		float4 posInLVP = mul(mLVP[cascadeIndex], float4(worldPos, 1.0f));
		posInLVP.xyz /= posInLVP.w;

		float depth = min(posInLVP.z, 1.0f);

		//uv座標変換。
		float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);

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
SPSIn VSMainCore(SVSIn vsIn, float4x4 wMat)
{
	SPSIn psIn;

	psIn.pos = mul(wMat, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	psIn.worldPos = psIn.pos.xyz;
	psIn.pos = mul(mView, psIn.pos);						//ワールド座標系からカメラ座標系に変換。
	psIn.pos = mul(mProj, psIn.pos);						//カメラ座標系からスクリーン座標系に変換。
	psIn.normal = normalize(mul(wMat, vsIn.normal));		//法線をワールド座標系に変換。
	psIn.Tangent = normalize(mul(wMat, vsIn.Tangent));
	psIn.biNormal = normalize(mul(wMat, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキン無しモデル用の頂点シェーダーのエントリーポイント。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, mWorld);
}
/// <summary>
/// スキン無しインスタンモデル用の頂点シェーダーのエントリーポイント。
/// </summary>
SPSIn VSMainInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
	return VSMainCore(vsIn, instancingDatas[instanceID]);
}

SPSIn VSMainSkinCore(SVSIn vsIn, float4x4 wMat)
{
	SPSIn psIn;

	float4x4 m;
	m = CalcSkinMatrix(vsIn.skinVert);
	m = mul(wMat, m);

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

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
SPSIn VSMainSkin(SVSIn vsIn)
{
	SPSIn psIn;

	float4x4 m;
	m = CalcSkinMatrix(vsIn.skinVert);

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

/*
	スキンありインスタンシングモデル用の頂点シェーダー。
*/
SPSIn VSMainSkinInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
	return VSMainSkinCore(vsIn, instancingDatas[instanceID]);
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
	float roughness = 1.0f;			//拡散反射の面の粗さ。
	for (int ligNo = 0; ligNo < numDirectionLight; ligNo++)
	{
		//ディファード拡散反射の色。
		float3 baseColor = max(dot(normal, -directionalLight[ligNo].direction), 0.0f) * directionalLight[ligNo].color.xyz;
		//DisneyModel拡散反射
		float disneyDiffuse = NormalizedDisneyDiffuse(normal, -directionalLight[ligNo].direction, toEye, roughness);
		float3 diffuse = baseColor * disneyDiffuse / PI;
		//クックトランスモデルの鏡面反射
		float3 specCol = CookTrranceSpecular(-directionalLight[ligNo].direction, toEye, normal, metaric) * directionalLight[ligNo].color.xyz;
		//拡散反射光と鏡面反射光を線形補完。
		lig += lerp(diffuse, specCol, metaric);
	}
	//環境光。
	lig += ambientLight;
	//シャドウ。
	float4 posInView = mul(mView, float4(psIn.worldPos,1.0f));
	float shadow = CalcShadow(psIn.worldPos, posInView.z);

	lig *= lerp(1.0f, 0.5f, shadow);

	//if (shadow == 1.0f)
	//{
	//	lig *= 0.5f;
	//}

	//最終的な色を決定する。
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}

//GBufferに書き込むピクセルシェーダーのエントリ関数。
PSOut_GBuffer PSMain_RenderGBuffer (SPSIn psIn){
	PSOut_GBuffer Out = (PSOut_GBuffer)0;

	Out.albedo = g_texture.SampleLevel(g_sampler, psIn.uv, 0);		//アルベド。
	//法線マップ。
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
	Out.normal.xyz = (normal / 2.0f) + 0.5f;

	//ワールド座標。
	Out.worldPos = float4(psIn.worldPos, 0.0f);

	//スペキュラマップ。
	Out.spec = 0.0f;
	if (hasSpecularMap) {
		//スペキュラマップがある。
		Out.spec = g_specularMap.Sample(g_sampler, psIn.uv).r;
	}

	//シャドウ。
	float4 posInView = mul(mView, float4(psIn.worldPos, 1.0f));
	Out.shadow = CalcShadow(psIn.worldPos, posInView.z);

	return Out;
}


/*
*	スキンなしモデルのシャドウマップ書き込み用の頂点シェーダー。
*/
SShadowMapPSIn VSMainNonSkinShadowMap(SShadowMapVSIn vsIn)
{
	SShadowMapPSIn psIn;
	psIn.pos = mul(mWorld, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	return psIn;
}

/*TODO:インスタンシング描画用のシャドウマップの頂点シェーダー作成。*/



/*
*	スキンなしインスタンシング描画モデルのシャドウマップ書き込み用の頂点シェーダー。
*/
SShadowMapPSIn VSMainNonSkinInstancingShadowMap(SShadowMapVSIn vsIn, uint instanceID : SV_InstanceID)
{
	SShadowMapPSIn psIn;
	psIn.pos = mul(instancingDatas[instanceID], vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

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
	psIn.pos = mul(mProj, psIn.pos);

	return psIn;
}
/*
*	シャドウマップ書き込み用のピクセルシェーダー。
*/
float4 PSMainShadowMap(SShadowMapPSIn psIn) :SV_Target0
{
	return psIn.pos.z / psIn.pos.w;
}
