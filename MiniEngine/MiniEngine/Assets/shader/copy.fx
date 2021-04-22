/*
	コピー用シェーダー。
*/

struct VSInput {
	float4 pos	: POSITION;
	float2 uv	: TEXCOORD0;
};

struct PSInput {
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};

Texture2D<float4> sceneTexture	: register(t0);		//シーンテクスチャ。
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
	PSinput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
	return sceneTexture.Sample(Sampler,In.uv);
}
