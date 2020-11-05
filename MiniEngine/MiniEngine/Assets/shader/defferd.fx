/*
	�f�B�t�@�[�h�V�F�[�f�B���O
*/

#include "modelCB.h"
#include "modelStruct.h"
#include "PBR.h"

//cbuffer PSDefferdCb : register(b0)
//{
//	float4x4 mViewProjInv;	//�r���[�v���W�F�N�V�����s��̋t�s��B
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

Texture2D<float4> g_albedoMap : register(t0);	//�A���x�h
Texture2D<float4> g_normalMap : register(t1);	//�@���B
Texture2D<float4> g_worldPosMap : register(t2);	//���[���h���W�B
Texture2D<float> g_specularMap : register(t3);	//�X�y�L�����B
Texture2D<float> g_shadowMap : register(t4);	//�V���h�E�B

StructuredBuffer<SDirectionalLight> directionalLight : register(t9);	//���C�g�B

//�T���v���X�e�[�g�B
sampler g_sampler : register(s0);

//�f�B�t�@�[�h�p�̒��_�V�F�[�_�[�B
PSDefferdInput VSMain(VSDefferdInput In)
{
	PSDefferdInput psIn;
	psIn.pos = mul(mvp,In.pos);
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSDefferdInput psIn) : SV_Target0
{
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);		//�A���x�h�B
	float3 normal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;	//�@���B
	normal = normal * 2.0f - 1.0f;
	float3 worldPos = g_worldPosMap.Sample(g_sampler, psIn.uv).xyz;	//���[���h���W�B
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv);	//�X�y�L�����B
	float shadow = g_shadowMap.Sample(g_sampler, psIn.uv);		//�V���h�E�B

	float3 lig = 0;		//���C�g
	float3 toEye = normalize(eyePos - worldPos);		//�_���王�_�܂ł̐��K���x�N�g��
	float roughness = 0.5f;			//�g�U���˂̖ʂ̑e���B
	for (int ligNo = 0; ligNo < numDirectionLight; ligNo++)
	{
		float3 baseColor = max(dot(normal, -directionalLight[ligNo].direction), 0.0f) * directionalLight[ligNo].color;
		//DisneyModel�g�U����
		lig += NormalizedDisneyDiffuse(baseColor, normal, -directionalLight[ligNo].direction, toEye, roughness);
		//�X�y�L��������
		lig += BRDF(-directionalLight[ligNo].direction, toEye, normal)
			* directionalLight[ligNo].color.xyz
			* metaric * directionalLight[ligNo].color.w;
	}
	//�����B
	lig += ambientLight;

	lig *= lerp(1.0f, 0.5f, shadow);

	//if (shadow == 1.0f)
	//{
	//	lig *= 0.5f;
	//}

	//�ŏI�I�ȐF�����肷��B
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}

