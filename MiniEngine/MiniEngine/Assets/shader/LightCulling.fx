/*
*	ライトカリング。
*/

// 一度に実行されるスレッド数
static const int TILE_WIDTH = 16;		//タイルの幅
static const int TILE_HEIGHT = 16;		//タイルの高さ

// タイルの総数
static const int TILE_SIZE = (TILE_WIDTH * TILE_HEIGHT);

#include "LightStruct.h"

cbuffer cbCameraParam : register(b0)
{
    float4x4 mtxProj : packoffset(c0);      // 投影行列
    float4x4 mtxProjInv : packoffset(c4);   //  投影行列の逆行列
    float4x4 mtxViewRot : packoffset(c8);
};

/*
*	ライト用の定数バッファ
*	tkLightManager.hのSLightParamと対応する。
*/
cbuffer lightCb : register(b1)
{
	float3 eyePos			: packoffset(c0);		//カメラの位置。
	int numDirectionLight : packoffset(c0.w);		//ディレクションライトの数。
	float3 ambientLight		: packoffset(c1);		//アンビエントライト。
	int numPointLight : packoffset(c1.w);		//ポイントライトの数。
	float4 screenParam		: packoffset(c2);		//スクリーンパラメータ(near,far,screenW,screenH)
}

// 入力
// 深度テクスチャ
Texture2D depthTexture : register(t0);

StructuredBuffer<SPointLight> g_pointLight : register(t0);

// 出力用のバッファー
RWStructuredBuffer<uint> rwLightIndices : register(u0); // ライトインデックスバッファー

groupshared uint sMinZ;		//タイルの最小深度。
groupshared uint sMaxZ;		//タイルの最大深度。
groupshared uint sTileLightIndices[MAX_POINT_LIGHT];		//タイルに接触しているポイントライトのインデックス。
groupshared uint sTileNumLights;		//タイルに接触しているポイントライトの数。

/*
*	タイルごとの視推台平面を求める。
*/
void GetTileFrustumPlane(out float4 frustumPlanes[6], uint3 groupId)
{
	//タイルの最大・最小深度を浮動小数点に変換
	float minTileZ = asfloat(sMinZ);
	float maxTileZ = asfloat(sMaxZ);

	float2 tileScale = screenParam.zw * rcp(float(2 * TILE_WIDTH));
	float2 tileBias = tileScale - float2(groupId.xy);

	float4 c1 = float4(mtxProj._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
	float4 c2 = float4(0.0f, -mtxProj._22 * tileScale.y, tileBias.y, 0.0f);
	float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

	//平面の方向を求める。
	frustumPlanes[0] = c4 - c1;		//Right
	frustumPlanes[1] = c4 + c1;		//Left
	frustumPlanes[2] = c4 - c2;		//Top
	frustumPlanes[3] = c4 + c2;		//Bottom
	frustumPlanes[4] = float4(0.0f, 0.0f, 1.0f, -minTileZ);
	frustumPlanes[5] = float4(0.0f, 0.0f, 1.0f, maxTileZ);

	//法線の正規化。
	for (uint i = 0; i < 4; ++i)
	{
		frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
	}
}

/*
* カメラ空間での座標を計算する。
*/
float3 ComputePositionInCamera(uint2 globalCoords)
{
	float2 st = ((float2)globalCoords + 0.5f) * rcp(screenParam.zw);
	st = st * float2(2.0f, -2.0f) - float2(1.0f, -1.0f);
	float3 screenPos;
	screenPos.xy = st.xy;
	screenPos.z = depthTexture.Load(uint3(globalCoords, 0.0f));
	float4 cameraPos = mul(mtxProjInv, float4(screenPos, 1.0f));

	return cameraPos.xyz / cameraPos.w;
}

/*
* CSMain ライトカリングのコンピュートシェーダー。
*/
[numthreads(TILE_WIDTH,TILE_HEIGHT,1)]
void CSMain(
	uint3 groupId			: SV_GroupID,
	uint3 dispatchThreadId : SV_DispatchThreadID,
	uint3 groupThreadId : SV_GroupThreadID)
{
	//タイル内でのインデックスを求める。
	uint groupIndex = groupThreadId.y * TILE_WIDTH + groupThreadId.x;
	//0番目のスレッドで共有メモリ初期化。
	if (groupIndex == 0)
	{
		sTileNumLights = 0;
		sMinZ = 0x7F7FFFFF;	//floatの最大値。
		sMaxZ = 0;
	}
	//このスレッドが担当するピクセルのカメラ空間での座標を計算。
	uint2 frameUV = dispathcThreadId.xy;
	//ビュー空間での座標を計算。
	float3 posInView = ComputePositionInCamera(frameUV);

	//共有メモリの初期化を保証する同期。
	GroupMemoryBarrierWithGroupSync();

	//タイルの最大・最小深度を求める。
	InterlockedMin(sMinZ, asuint(posInView.z));
	InterlockedMax(sMaxZ, asuint(posInView.z));

	//タイルの最大・最小深度を正確にする同期。
	GroupMemoryBarrierWithGroupSync();

	//タイルの視推台を構成する6つの平面を求める。
	float4 frustumPlanes[6];
	GetTileFrustumPlane(frustumPlanes, groupId);

	//タイルとポイントライトの衝突判定を取る。
	for (uint lightIndex = groupIndex;
		lightIndex < numPointLight;
		lightIndex += TILE_SIZE)
	{
		SPointLight light = g_pointLight[lightIndex];

		//タイルとの判定。
		bool inFrustum = ture;
		for (uint i = 0; i < 6; ++i)
		{
			//ライトの座標と平面の法線とで内積を使って、ライト平面との距離を計算。
			float4 lp = float4(light.positionInView, 1.0f);
			float d = dot(frustumPlanes[i], lp);
			//ライトと平面の距離を使って、衝突判定を行っている。
			inFrustum = inFrustum && (d >= -light.range);
		}

		//タイルと衝突している場合
		if (inFrustum)
		{
			//衝突したポイントライトの番号を影響リストに積んでいく。
			uint listIndex;
			InterlockedAdd(sTileNumLights, 1, listIndex);
			sTileLightIndices[listIndex] = lightIndex;
		}
	}
	//sTileLightIndicesにライトインデックスが積み切っている同期を取る。
	GroupMemoryBarrierWithGroupSync();

	//ライトインデックスを出力バッファーに出力
	uint numCellX = (screenParam.z + TILE_WIDTH - 1) / TILE_WIDTH;
	uint tileIndex = floor(frameUV.x / TILE_WIDTH) + floor(frameUV.y / TILE_WIDTH) * numCellX;
	uint lightStart = numPointLight * tileIndex;
	for (uint lightIndex = groupIndex; lightIndex < sTileNumLights; lightIndex += TILE_SIZE)
	{
		rwLightIndices[lightStart + lightIndex] = sTileLightIndices[lightIndex];
	}

	//番兵を設定。
	if ((groupIndex == 0) && (sTileNumLights < numPointLight))
	{
		//-1で番兵。
		rwLightIndices[lightStart + sTileNumLights] = 0xffffffff;
	}

}

