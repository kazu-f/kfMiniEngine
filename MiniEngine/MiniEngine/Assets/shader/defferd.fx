/*
	ディファードシェーディング
*/

#include "modelCB.h"
#include "modelStruct.h"
#include "PBR.h"
#include "Fog.h"

//cbuffer PSDefferdCb : register(b0)
//{
//	float4x4 mViewProjInv;	//ビュープロジェクション行列の逆行列。
//};

cbuffer cb : register(b0) {
	float4x4 mvpInv;
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
Texture2D<float4> g_specularMap : register(t3);	//スペキュラ。
Texture2D<float> g_shadowMap : register(t4);	//シャドウ。
Texture2D<float> g_reflectionMap : register(t5);	//反射率。
TextureCube<float4> g_cubeMap : register(t6);	//キューブマップ。

StructuredBuffer<SDirectionalLight> directionalLight : register(t7);	//ライト。

//サンプラステート。
sampler g_sampler : register(s0);

//ライトの輝度。
static const float LIGHT_LUMINANCE = float3(0.298912, 0.586611, 0.114478);

//ディファード用の頂点シェーダー。
PSDefferdInput VSMain(VSDefferdInput In)
{
	PSDefferdInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSDefferdInput psIn) : SV_Target0
{
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);		//アルベド。
	
	float3 normal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;	//法線。
	normal = normal * 2.0f - 1.0f;
	float3 worldPos = g_worldPosMap.Sample(g_sampler, psIn.uv).xyz;	//ワールド座標。
	float4 specMap = g_specularMap.Sample(g_sampler, psIn.uv);	//スペキュラ。
	float shadow = g_shadowMap.Sample(g_sampler, psIn.uv);		//シャドウ。
	float reflection = g_reflectionMap.Sample(g_sampler, psIn.uv);		//シャドウ。

	float3 lig = 0;		//ライト
	float3 toEye = (eyePos - worldPos);		//点から視点までの正規化ベクトル
	float len = length(toEye);
	toEye = normalize(toEye);
	float spec = specMap.a;			//スペキュラマップのrチャンネルにスペキュラ強度。
	float roughness = 1.0f - spec;			//拡散反射の面の粗さ。
	float metaric = specMap.r;		//gチャンネルに金属度。

	for (int ligNo = 0; ligNo < numDirectionLight; ligNo++)
	{
		if (shadow < 0.9f) {
			//ランバート拡散反射の色。
			float3 baseColor = saturate(dot(normal, -directionalLight[ligNo].direction)) * directionalLight[ligNo].color.xyz / PI;
			//DisneyModel拡散反射
			float disneyDiffuse = NormalizedDisneyDiffuse(normal, -directionalLight[ligNo].direction, toEye, roughness);
			float3 diffuse = baseColor * disneyDiffuse;

			//return float4(diffuse, 1.0f);
			
			//クックトランスモデルの鏡面反射
			//float3 specCol = CookTrranceSpecular(-directionalLight[ligNo].direction, toEye, normal, metaric) * directionalLight[ligNo].color.xyz;
			float ligLum = dot(LIGHT_LUMINANCE, directionalLight[ligNo].color.xyz);
			float specPower = CookTrranceSpecular(-directionalLight[ligNo].direction, toEye, normal, spec);
			//鏡面反射光の光は金属度が上がると、材質の色になる。
			float3 specCol = lerp(directionalLight[ligNo].color.xyz, albedoColor * ligLum, metaric) * specPower;
			//return float4(specCol, 1.0f);
			//拡散反射光と鏡面反射光を線形補完。
			lig += lerp(diffuse, specCol, spec);

			//return float4(lig, 1.0f);

			//float specTerm = length(albedoColor.xyz * metaric);
			//specCol *= lerp(float3(specTerm, specTerm, specTerm), albedoColor.xyz, metaric);
			//lig += diffuse * (1.0f - specTerm) + specCol;
		}
	}
	//環境光。
	lig += ambientLight;

	//最終的な色を決定する。
	
	float4 refColor = g_cubeMap.Sample(g_sampler, reflect(toEye,normal));
	reflection *= refColor.a;
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor * (1.0f - reflection) + refColor.xyz * reflection;
	
	finalColor.xyz = finalColor.xyz * lig;

	//霧の計算。
	finalColor = CalcFog(finalColor, len);

	finalColor.a = albedoColor.a;
	return finalColor;
}

