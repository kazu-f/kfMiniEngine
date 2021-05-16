/*
*	スカイキューブシェーダー。
*/

#include "modelCB.h"

//頂点シェーダーへの入力。
struct SVSIn_SkyCube {
	float4 pos : POSITION;		//モデルの頂点座標。
	float3 normal : NORMAL;		//法線。
}

struct SPSIn_SkyCube {
	float4 pos : SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal : NORMAL;		//法線。
};

//空用の頂点シェーダー。
SPSIn_SkyCube VSMain_SkyCube(SVSIn_SkyCube In) {
	SPSIn_SkyCube psIn;

	psIn.pos = mul(mWorld, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	psIn.pos = mul(mView, psIn.pos);						//ワールド座標系からカメラ座標系に変換。
	psIn.pos = mul(mProj, psIn.pos);						//カメラ座標系からスクリーン座標系に変換。
	psIn.nomal = In.nomal;								//法線。

	return psIn;
}

//空用のピクセルシェーダー。
float4 PSMain_SkyCube(SPSIn_SkyCube In) : SV_Target0
{
	float4 color = skyCubeMap.Sample(Sampler, In.Normal);
	return color;
}
