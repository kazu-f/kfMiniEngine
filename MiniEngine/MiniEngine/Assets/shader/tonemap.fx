/*
*	�g�[���}�b�v�B
*/

struct VSInput {
	float4 pos  : POSITION;
	float2 uv   : TEXCOORD0;
};

struct PSInput {
	float4 pos  : SV_POSITION;
	float2 uv   : TEXCOORD0;
};

//�g�[���}�b�v�̋��ʒ萔�o�b�t�@�B
cbuffer cbTonemapCommon : register(b1) {
	float deltaTime;
	float middleGray;
};

/*
*	���_�V�F�[�_�[�B
*/
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

Texture2D<float4> sceneTexture : register(t0);		//�V�[���e�N�X�`���B
sampler Sampler : register(s0);

//�P�x�̑ΐ����ς����߂�B

static const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);
static const int MAX_SAMPLES = 16;

/*
*	�萔�o�b�t�@�B
*/
cbuffer cbCalcLuminanceLog : register(b0) {
	float4 g_avSampleOffsets[MAX_SAMPLES];
};

/*
*	�P�x�̑ΐ����ς����߂�B
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
*	���ϋP�x�v�Z�s�N�Z���V�F�[�_�[�B
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
*	�w���֐����g�p���ĕ��ϋP�x�����߂�s�N�Z���V�F�[�_�[�B
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
*	���Ï����B
*/

Texture2D<float4> lumAvgTexture : register(t1);		//���ϋP�x�B
Texture2D<float4> lastLumAvgTexture : register(t2);	//��t���[���O�̕��ϋP�x�B

/*
*	���Ï����̂��߂̕��ϋP�x�̓K��������s�N�Z���V�F�[�_�[�B
*/
float4 PSCalcAdaptedLuminance(PSInput In) : SV_TARGET0
{
	float fAdaptedLum = lastLumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));
	float fCurrentLum = lumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));

	float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * ( 1 - pow(0.98f,60*deltaTime));
	
	return float4(fNewAdaptation,fNewAdaptation,fNewAdaptation,1.0f);
}
/*
*	���Ï����̂��߂̃s�N�Z���V�F�[�_�[�B(�V�[�����؂�ւ�������Ɏg�p����B)
*/
float4 PSCalcAdaptedLuminanceFirst(PSInput In):SV_TARGET0
{
	float fAvgLum = lumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));
	return float4(fAvgLum,fAvgLum,fAvgLum,1.0f);
}

/*
*	���ϋP�x����g�[���}�b�v���s���s�N�Z���V�F�[�_�[�B
*/
float4 PSFinal(PSInput In) : SV_TARGET0
{
	float4 vSample = sceneTexture.Sample(Sampler,In.uv);
	float fAvgLum = lumAvgTexture.Sample(Sampler,float2(0.5f,0.5f));

	vSample.rgb *= middleGray / max(0.0001f,fAvgLum);
	//vSample.rgb /= (1.0f+vSample);
	return vSample;
}

