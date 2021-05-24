/*
*	トーンマップ。
*/

struct VSInput {
	float4 pos  : POSITION;
	float2 uv   : TEXCOORD0;
};

struct PSInput {
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
};

//トーンマップの共通定数バッファ。
cbuffer cbTonemapCommon : register(b1) {
	float deltaTime;
	float middleGray;
};

/*
*	頂点シェーダー。
*/
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

Texture2D<float4> sceneTexture : register(t0);		//シーンテクスチャ。
sampler Sampler : register(s0);

//輝度の対数平均を求める。

static const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);
static const int MAX_SAMPLES = 16;

/*
*	定数バッファ。
*/
cbuffer cbCalcLuminanceLog : register(b0) {
	float4 g_avSampleOffsets[MAX_SAMPLES];
};

/*
*	輝度の対数平均を求める。
*/
float4 PSCalcLuminanceLogAverage(PSInput In) : SV_TARGET0
{
	//return sceneTexture.Sample(Sampler,In.uv);

	float3 vSampler = 0.0f;
	float fLogLumSum = 0.0f;

	for (int iSample = 0; iSample < 9; iSample++)
	{
		vSampler = sceneTexture.Sample(Sampler, In.uv + g_avSampleOffsets[iSample].xy);
		float d = dot(vSampler, LUMINANCE_VECTOR) + 0.0001f;
		fLogLumSum += log(d);
	}
	
	fLogLumSum /= 9;

	return float4(fLogLumSum, fLogLumSum, fLogLumSum, 1.0f);
}

/*
*	平均輝度計算ピクセルシェーダー。
*/
float4 PSCalcLuminanceAverage(PSInput In) : SV_TARGET0
{

	float fResampleSum = 0.0f;
	
	for (int iSample = 0; iSample < 16; iSample++) 
	{
		fResampleSum += sceneTexture.Sample(Sampler, In.uv + g_avSampleOffsets[iSample].xy);
	}

	fResampleSum /= 16;

	return float4(fResampleSum, fResampleSum, fResampleSum, 1.0f);
}

/*
*	指数関数を使用して平均輝度を求めるピクセルシェーダー。
*/
float4 PSCalcLuminanceExpAverage(PSInput In) : SV_TARGET0
{
	float fResampleSum = 0.0f;
	
	for (int iSample = 0; iSample < 16; iSample++)
	{
		fResampleSum += sceneTexture.Sample(Sampler, In.uv + g_avSampleOffsets[iSample]);
	}

	fResampleSum = exp(fResampleSum / 16);

	return float4(fResampleSum, fResampleSum, fResampleSum, 1.0f);
}

/*
*	明暗順応。
*/

Texture2D<float4> lumAvgTexture : register(t1);		//平均輝度。
Texture2D<float4> lastLumAvgTexture : register(t2);	//一フレーム前の平均輝度。

/*
*	明暗順応のための平均輝度の適合させるピクセルシェーダー。
*/
float4 PSCalcAdaptedLuminance(PSInput In) : SV_TARGET0
{
	float fAdaptedLum = lastLumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));
	float fCurrentLum = lumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));

	float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * ( 1 - pow(0.98f,60*deltaTime));
	
	return float4(fNewAdaptation,fNewAdaptation,fNewAdaptation,1.0f);
}
/*
*	明暗順応のためのピクセルシェーダー。(シーンが切り替わった時に使用する。)
*/
float4 PSCalcAdaptedLuminanceFirst(PSInput In):SV_TARGET0
{
	float fAvgLum = lumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));
	return float4(fAvgLum,fAvgLum,fAvgLum,1.0f);
}

/*
*	平均輝度からトーンマップを行うピクセルシェーダー。
*/
float4 PSFinal(PSInput In) : SV_TARGET0
{
	float4 vSample = sceneTexture.Sample(Sampler,In.uv);
	float fAvgLum = lumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));

	vSample.rgb *= middleGray / max(0.0001f,fAvgLum);
	//vSample.rgb /= (1.0f+vSample);
	return vSample;
}

