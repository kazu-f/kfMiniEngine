/*
*	FXAA
*/

struct VSInput {
	float4 pos  : POSITION;
	float2 uv   : TEXCOORD0;
};

struct PSInput {
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
};

Texture2D<float4> sceneTexture : register(t0);	//シーンテクスチャ。
sampler g_sampler : register(s0);

//アンチの処理。
float4 FxaaPixelShader(
	float2 pos,
	float4 fxaaConsoleRcpFrameOpt,
	float4 fxaaConsoleRcpFrameOpt2,
	float fxaaQualityEdgeThreshold,
	float fxaaQualityEdgeThresholdMin,
	float fxaaConsoleEdgeSharpness,
	float fxaaConsoleEdgeThreshold,
	float fxaaConsoleEdgeThresholdMin,		//輝度の差の閾値の最低値。
	float2 sceneTexSize
)
{
	//近傍4テクセルをフェッチ。
	float4 nTex = float4(
		pos.x + 0.5f / sceneTexSize.x,
		pos.y + 0.5f / sceneTexSize.y,
		pos.x - 0.5f / sceneTexSize.x,
		pos.y - 0.5f / sceneTexSize.y
		);

	float lumaNw = sceneTexture.Sample(g_sampler, nTex.xy).y;	//X+ Y+
	float lumaSw = sceneTexture.Sample(g_sampler, nTex.xw).y;	//X+ Y-
	float lumaNe = sceneTexture.Sample(g_sampler, nTex.zy).y;	//X- Y+
	float lumaSe = sceneTexture.Sample(g_sampler, nTex.zw).y;	//X- Y-

	float4 rgbyM = sceneTexture.Sample(g_sampler,pos.xy);
	float lumaM = rgbyM.y;

	//近傍テクセルの輝度の差を調べる。
	//NwとSwの比較。
	float lumaMaxNwSw = max(lumaNw,lumaSw);
	lumaNe += 1.0/384.0;		//謎の式。必要か？
	float lumaMinNwSw = min(lumaNw,lumaSw);
	//NeとSeの比較。
	float lumaMaxNeSe = max(lumaNe,lumaSe);
	float lumaMinNeSe = min(lumaNe,lumaSe);
	//比較したものをさらに比較。
	float lumaMax = max(lumaMaxNwSw,lumaMaxNeSe);
	float lumaMin = min(lumaMinNwSw,lumaMinNeSe);
	//輝度の差の閾値。
	float lumaMaxScaled = lumaMax * fxaaConsoleEdgeThreshold;
	float lumaMaxScaledClamped = max(fxaaConsoleEdgeThresholdMin,lumaMaxScaled);	//最終的な輝度の差の閾値。
	//輝度の最低値、最大値。
	float lumaMinM = min(lumaMin,lumaM);		//輝度の最低値を求める。
	float lumaMaxM = min(lumaMax,lumaM);		//輝度の最大値を求める。
	//輝度の差を求める。
	float dirSwMinusNe = lumaSw - lumaNe;		//Sw - Me
	float lumaMaxSubMinM = lumaMaxM - lumaMinM;	//最大値 - 最低値。
	float dirSeMinusNw = lumaSe - lumaNw;		//Se - Nw
	//輝度の差は閾値以下か？
	if(lumaMaxSubMinM < lumaMaxScaledClamped){
		//閾値以下だったため、アンチをかけない。
		//return float4(0.0f, 0.0f, 0.0f, 1.0f);
		return rgbyM;
	}
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	//輝度の差を利用して、ギザギザが発生している可能性の高いテクセルをフェッチする。
	float2 dir;
	dir.x = dirSwMinusNe + dirSeMinusNw;
	dir.y = dirSwMinusNe - dirSeMinusNw;
	//dir.x = -((lumaNw + lumaNe) - (lumaSw + lumaSe));
	//dir.y = ((lumaNw + lumaSw) - (lumaNe + lumaSe));

	float2 dir1 = normalize(dir.xy);
	float4 rgbyN1 = sceneTexture.Sample(g_sampler, pos.xy - dir1 * fxaaConsoleRcpFrameOpt.zw);
	float4 rgbyP1 = sceneTexture.Sample(g_sampler, pos.xy + dir1 * fxaaConsoleRcpFrameOpt.zw);

	float dirAbsMinTimesC = min(abs(dir1.x),abs(dir1.y)) * fxaaConsoleEdgeSharpness;
	float2 dir2 = clamp(dir1.xy / dirAbsMinTimesC, -2.0, 2.0);

	float4 rgbyN2 = sceneTexture.Sample(g_sampler, pos.xy - dir2 * fxaaConsoleRcpFrameOpt2.zw);
	float4 rgbyP2 = sceneTexture.Sample(g_sampler, pos.xy + dir2 * fxaaConsoleRcpFrameOpt2.zw);
	//ブレンド
	float4 rgbyA = rgbyN1 + rgbyP1;
	//return rgbyA * 0.5f;
	float4 rgbyB = ((rgbyN2 + rgbyP2) * 0.25) + (rgbyA * 0.25);

	int twoTap = (rgbyB.y < lumaMin) || (rgbyB.y > lumaMax);

	if(twoTap){
		//輝度の差がまだ大きいためブレンド。
		rgbyB.xyz = rgbyA.xyz * 0.5;

		//return float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	return rgbyB;
}

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSInput In) : SV_TARGET0
{
	float2 texSize;
	float level;
	sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);	//テクスチャのサイズを取得。
	float4 rcpFrame = float4(0.0f, 0.0f, 1.0f/texSize.x, 1.0f/texSize.y);
	return FxaaPixelShader(
		In.uv,
		rcpFrame,			//fxaaConsoleRcpFrameOpt
		rcpFrame,			//fxaaConsoleRcpFrameOpt2
		0.166f,				//fxaaQualityEdgeThreshold
		0.0833f,			//fxaaQualityEdgeThresholdMin
		1.0f,				//fxaaConsoleEdgeSharpness
		0.4f,				//fxaaConsoleEdgeThreshold
		0.0833f,			//fxaaConsoleEdgeThresholdMin
		texSize				//テクスチャのサイズ。
	);

	//return FxaaPixelShader(
	//	In.uv,
	//	rcpFrame,			//fxaaConsoleRcpFrameOpt
	//	rcpFrame,			//fxaaConsoleRcpFrameOpt2
	//	0.166f,				//fxaaQualityEdgeThreshold
	//	0.0833f,			//fxaaQualityEdgeThresholdMin
	//	1.0f,				//fxaaConsoleEdgeSharpness
	//	0.125,				//fxaaConsoleEdgeThreshold
	//	0.0312,			//fxaaConsoleEdgeThresholdMin
	//	texSize				//テクスチャのサイズ。
	//);
}


