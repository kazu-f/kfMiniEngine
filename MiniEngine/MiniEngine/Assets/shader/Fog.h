/*
*	フォグ。
*/

static const float FOG_START	=  40000.0f;		//フォグが掛かり始める距離。
static const float FOG_END		= 150000.0f;		//フォグが完全に掛かる距離。
static const float4 FOG_COLOR = float4(0.9f, 0.9f, 0.9f, 1.0f);		//フォグの色。

float4 CalcFog(float4 color, float dist)
{
	float fog = (FOG_END - dist) / (FOG_END - FOG_START);
	fog = max(0.0f, min(1.0f, fog));
	float4 finalColor = color * fog + FOG_COLOR * (1.0f - fog);
	return finalColor;
}

/*
*	スカイキューブ用のフォグ。
*/

static const float SKYFOG_START	= 2500.0f;		//フォグを開始する高さ。
static const float SKYFOG_END	= 100.0f;		//フォグが完全に掛かる高さ。

float4 CalcSkyFog(float4 color, float height)
{
	float fog = (SKYFOG_END - height) / (SKYFOG_END - SKYFOG_START);
	fog = max(0.0f, min(1.0f, fog));

	float4 finalColor = color * fog + FOG_COLOR * (1.0f - fog);
	return finalColor;
}


