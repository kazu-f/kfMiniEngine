/*
*		モデルシェーダー。
*/

#include "modelCB.h"
#include "modelStruct.h"

static const float PI = 3.14159265358979323846;

//static const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの本数。

////ライト用の定数バッファ。
//cbuffer LightCb : register(b2) {
//	SDirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
//	float3 eyePos;					//カメラの視点。
//	float specPow;					//スペキュラの絞り。
//	float3 ambinentLight;			//環境光。
//};

//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
//ボーン行列。
StructuredBuffer<float4x4> boneMatrix : register(t3);
//ディレクションライト。
StructuredBuffer<SDirectionalLight> directionalLight : register(t4);

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
*	albedColor	:	ライトの強さかなぁ？
*	N			:	法線の正規化ベクトル
*	L			:	ライトへの正規化ベクトル
*	V			:	視点の正規化ベクトル
*	roughness	:	表面の粗さを表すパラメータらしい
*/
float3 NormalizedDisneyDiffuse(float3 baseColor, float3 N, float3 L, float3 V, float roughness)
{
	float3 H = normalize(L + V);		//ハーフベクトル。

	float energyBias = lerp(0.0f, 0.5f, roughness);				//なんか正規化のための数値？0.0〜0.5の線形補完
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);	//同じく？なんかメンドイ線形補完してる感じする。
	//内積する。(下限0.0〜上限1.0)
	float LdotH = saturate(dot(L, H));		//ライトとハーフベクトル
	float NdotL = saturate(dot(N, L));		//法線とライト
	float NdotV = saturate(dot(N, V));		//法線と視点

	//謎数値　フレネル反射率?(0.0〜1.0)
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

/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float3 lig = 0.0f;
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	//////////////////////////////////////////////////////
	// 拡散反射を計算
	//////////////////////////////////////////////////////
	{
		for (int i = 0; i < numDirectionLight; i++) {
			float NdotL = dot(psIn.normal, -directionalLight[i].direction);	//ライトの逆方向と法線で内積を計算する。
			if (NdotL < 0.0f) {	//内積の計算結果はマイナスになるので、if文で判定する。
				NdotL = 0.0f;
			}
			float3 diffuse;
			diffuse = directionalLight[i].color.xyz * (1.0f - metaric) * NdotL; //拡散反射光を足し算する。

			//return float4( diffuse, 1.0f);		//なんか拡散反射レベルを見たかったんかこれ？

			//ライトをあてる物体から視点に向かって伸びるベクトルを計算する。
			float3 eyeToPixel = eyePos - psIn.worldPos;
			eyeToPixel = normalize(eyeToPixel);

			//光の物体に当たって、反射したベクトルを求める。
			float3 reflectVector = reflect(directionalLight[i].direction, psIn.normal);
			//反射した光が目に飛び込んて来ているかどうかを、内積を使って調べる。
			float d = dot(eyeToPixel, reflectVector);
			if (d < 0.0f) {
				d = 0.0f;
			}
			//d = pow(d, specPow) * metaric;
			d = pow(d, 5.0f) * metaric;		//仮置き。
			float3 spec = directionalLight[i].color.xyz * d * 5.0f;
			//スペキュラ反射の光を足し算する。
			lig += diffuse + spec;
		}
	}

	//////////////////////////////////////////////////////
	// 環境光を計算
	//////////////////////////////////////////////////////
	lig += ambientLight; //足し算するだけ

	float4 texColor = g_texture.Sample(g_sampler, psIn.uv);
	texColor.xyz *= lig; //光をテクスチャカラーに乗算する。
	return float4(texColor.xyz, 1.0f);
}

//物理ベースライティングのピクセルシェーダー。
float4 PSMainBPR(SPSIn psIn) : SV_Target0
{
	//法線の計算。
	float3 normal;
	if (hasNormalMap) {
		//法線マップから法線を引っ張ってくる。
		float3 binSpaceNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;	//-1.0f〜1.0fに調整。
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

	//最終的な色を決定する。
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
