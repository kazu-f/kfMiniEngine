/*
	ディファードシェーディング
*/

#include "modelCB.h"
#include "modelStruct.h"
#include "PBR.h"

//cbuffer PSDefferdCb : register(b0)
//{
//	float4x4 mViewProjInv;	//ビュープロジェクション行列の逆行列。
//};

cbuffer cb : register(b0) {
	float4x4 mvp;		
	float4 mulColor;	
};

struct VSDefferdInput {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSDefferdInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> g_albedoMap : register(t0);	//アルベド
Texture2D<float4> g_normalMap : register(t1);	//法線。
Texture2D<float4> g_worldPosMap : register(t2);	//ワールド座標。
Texture2D<float> g_specularMap : register(t3);	//スペキュラ。
Texture2D<float> g_shadowMap : register(t4);	//シャドウ。

StructuredBuffer<SDirectionalLight> directionalLight : register(t9);	//ライト。

//サンプラステート。
sampler g_sampler : register(s0);

//ディファード用の頂点シェーダー。
PSDefferdInput VSMain(VSDefferdInput In)
{
	PSDefferdInput psIn;
	psIn.pos = mul(mvp,In.pos);
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSDefferdInput psIn) : SV_Target0
{
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);		//アルベド。
	float3 normal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;	//法線。
	normal = normal * 2.0f - 1.0f;
	float3 worldPos = g_worldPosMap.Sample(g_sampler, psIn.uv).xyz;	//ワールド座標。
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv);	//スペキュラ。
	float shadow = g_shadowMap.Sample(g_sampler, psIn.uv);		//シャドウ。

	float3 lig = 0;		//ライト
	float3 toEye = normalize(eyePos - worldPos);		//点から視点までの正規化ベクトル
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

