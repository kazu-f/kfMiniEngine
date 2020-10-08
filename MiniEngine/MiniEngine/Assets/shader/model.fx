/*
*		���f���V�F�[�_�[�B
*/

#include "modelCB.h"
#include "modelStruct.h"

static const float PI = 3.14159265358979323846;

//static const int NUM_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̖{���B

////���C�g�p�̒萔�o�b�t�@�B
//cbuffer LightCb : register(b2) {
//	SDirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
//	float3 eyePos;					//�J�����̎��_�B
//	float specPow;					//�X�y�L�����̍i��B
//	float3 ambinentLight;			//�����B
//};

//���f���e�N�X�`���B
Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
//�{�[���s��B
StructuredBuffer<float4x4> boneMatrix : register(t3);
//�f�B���N�V�������C�g�B
StructuredBuffer<SDirectionalLight> directionalLight : register(t4);

//�T���v���X�e�[�g�B
sampler g_sampler : register(s0);

//�x�b�N�}�����z?	
/*
	m			(�ʂ̑e��)
	t = N�EH	(�@���E�n�[�t�x�N�g��)
	M = m^2		T = (N�EH)^2
*/
float Beckmann(float m, float t)
{
	float M = m * m;
	float T = t * t;
	return exp((T - 1) / (M * T)) / (M * M * T);
}

//�t���l����?�̋ߎ����炵���H
float specFresnel(float f0, float u)
{
	return f0 + (1 - f0) * pow(1 - u, 5);
}

/*
*	BRDF(�o�������˗����z�֐�)
*	�X�y�L�����̌v�Z�Ɏg���Ă�B�H
*	L�����C�g
*	V�����_	
*	N���@��
*/
float BRDF(float3 L, float3 V, float3 N)
{
	float microfacet = 1.0f;		//�}�C�N���t�@�Z�b�g
									//�\�ʂ̓ʉ����\���I�ȁH
									//�ʂ̑e���Ƃ��𒲐�����p�����[�^�炵���H
	float f0 = 0.5f;				//��̐����B
									//�������ˎ��̃t���l�����ˌW��???
	
	float3 H = normalize(L + V);		//���C�g+���_�̃n�[�t�x�N�g���B

	//�F�X���ώ���Ă�B
	float NdotH = dot(N, H);		//�@���ƃn�[�t�x�N�g��
	float VdotH = dot(V, H);		//���_�ƃn�[�t�x�N�g��
	float NdotL = dot(N, L);		//�@���ƃ��C�g
	float NdotV = dot(N, V);		//�@���Ǝ��_

	float D = Beckmann(microfacet, NdotH);	//�����ʕ��z�֐��B
	float F = specFresnel(f0, VdotH);		//�t���l�����̋ߎ����B

	float t = 2.0 * NdotH / VdotH;			//�v�Z�̋��ʍ�������Ă��������B
	float G = max( 0.0f, min(1.0f,min(t * NdotV, t * NdotL)) );		//�􉽊w�I�����W���H
																	//�ő�l��1.0�A�ŏ��l��0.0�̊Ԃ�
																	//�l�̏������ق������

	float m = PI * NdotV * NdotL;			//�N�b�N�g�����X���˃��f���̕���ɂȂ�l�H	

	/*
	*	F:�t���l����
	*	D:�����ʕ��z
	*	G:�􉽊w�I�����W��
	*/
	return max(F * D * G / m, 0.0f);
}

/*
*	�t���l�����H
*	�Ȃ񂩏����Ⴄ�H1.0�̕������ϐ��ɂȂ��Ă�H
*/
float SchlickFresnel(float u, float f0, float f90)
{
	//f90��1.0�Ȃ�t���l�����ł��˂��B
	return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

/*
*	���K��Disney���f���g�U���ˁB
*	albedColor	:	���C�g�̋������Ȃ��H
*	N			:	�@���̐��K���x�N�g��
*	L			:	���C�g�ւ̐��K���x�N�g��
*	V			:	���_�̐��K���x�N�g��
*	roughness	:	�\�ʂ̑e����\���p�����[�^�炵��
*/
float3 NormalizedDisneyDiffuse(float3 baseColor, float3 N, float3 L, float3 V, float roughness)
{
	float3 H = normalize(L + V);		//�n�[�t�x�N�g���B

	float energyBias = lerp(0.0f, 0.5f, roughness);				//�Ȃ񂩐��K���̂��߂̐��l�H0.0�`0.5�̐��`�⊮
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);	//�������H�Ȃ񂩃����h�C���`�⊮���Ă銴������B
	//���ς���B(����0.0�`���1.0)
	float LdotH = saturate(dot(L, H));		//���C�g�ƃn�[�t�x�N�g��
	float NdotL = saturate(dot(N, L));		//�@���ƃ��C�g
	float NdotV = saturate(dot(N, V));		//�@���Ǝ��_

	//�䐔�l�@�t���l�����˗�?(0.0�`1.0)
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * roughness;
	
	//���ꂾ�����畁�ʂɃt���l�����g����ł�����ł́H
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);	//�t���l���W��(�@���E���C�g)
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);	//�t���l���W��(�@���E���_)

	/*
	*	�悭�킩���W�� * ���C�g�̋����H 
	*	* �t���l���W��(N�EL) * �t���l���W��(N�EV) / PI
	*/
	return (energyFactor * baseColor * FL * FV) / PI;
}

//�X�L���s����v�Z����֐��B
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

//���_�V�F�[�_�[�̃R�A�֐��B
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

	psIn.pos = mul(m, vsIn.pos);						//���f���̒��_�����[���h���W�n�ɕϊ��B
	psIn.worldPos = psIn.pos.xyz;
	psIn.pos = mul(mView, psIn.pos);						//���[���h���W�n����J�������W�n�ɕϊ��B
	psIn.pos = mul(mProj, psIn.pos);						//�J�������W�n����X�N���[�����W�n�ɕϊ��B
	psIn.normal = normalize(mul(m, vsIn.normal));		//�@�������[���h���W�n�ɕϊ��B
	psIn.Tangent = normalize(mul(m, vsIn.Tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
	return VSMainCore(vsIn, false);
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
SPSIn VSMainSkin(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float3 lig = 0.0f;
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	//////////////////////////////////////////////////////
	// �g�U���˂��v�Z
	//////////////////////////////////////////////////////
	{
		for (int i = 0; i < numDirectionLight; i++) {
			float NdotL = dot(psIn.normal, -directionalLight[i].direction);	//���C�g�̋t�����Ɩ@���œ��ς��v�Z����B
			if (NdotL < 0.0f) {	//���ς̌v�Z���ʂ̓}�C�i�X�ɂȂ�̂ŁAif���Ŕ��肷��B
				NdotL = 0.0f;
			}
			float3 diffuse;
			diffuse = directionalLight[i].color.xyz * (1.0f - metaric) * NdotL; //�g�U���ˌ��𑫂��Z����B

			//return float4( diffuse, 1.0f);		//�Ȃ񂩊g�U���˃��x���������������񂩂���H

			//���C�g�����Ă镨�̂��王�_�Ɍ������ĐL�т�x�N�g�����v�Z����B
			float3 eyeToPixel = eyePos - psIn.worldPos;
			eyeToPixel = normalize(eyeToPixel);

			//���̕��̂ɓ������āA���˂����x�N�g�������߂�B
			float3 reflectVector = reflect(directionalLight[i].direction, psIn.normal);
			//���˂��������ڂɔ�э���ė��Ă��邩�ǂ������A���ς��g���Ē��ׂ�B
			float d = dot(eyeToPixel, reflectVector);
			if (d < 0.0f) {
				d = 0.0f;
			}
			//d = pow(d, specPow) * metaric;
			d = pow(d, 5.0f) * metaric;		//���u���B
			float3 spec = directionalLight[i].color.xyz * d * 5.0f;
			//�X�y�L�������˂̌��𑫂��Z����B
			lig += diffuse + spec;
		}
	}

	//////////////////////////////////////////////////////
	// �������v�Z
	//////////////////////////////////////////////////////
	lig += ambientLight; //�����Z���邾��

	float4 texColor = g_texture.Sample(g_sampler, psIn.uv);
	texColor.xyz *= lig; //�����e�N�X�`���J���[�ɏ�Z����B
	return float4(texColor.xyz, 1.0f);
}

//�����x�[�X���C�e�B���O�̃s�N�Z���V�F�[�_�[�B
float4 PSMainBPR(SPSIn psIn) : SV_Target0
{
	//�@���̌v�Z�B
	float3 normal;
	if (hasNormalMap) {
		//�@���}�b�v����@�������������Ă���B
		float3 binSpaceNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;	//-1.0f�`1.0f�ɒ����B
		normal = psIn.Tangent * binSpaceNormal.x		//�ڐ�(�@���ɑ΂��ĉE�H)
			+ psIn.biNormal * binSpaceNormal.y			//�]�@��(�@���ɑ΂��ď�)
			+ psIn.normal * binSpaceNormal.z;			//�@������
	}
	else {
		normal = psIn.normal;
	}
	float3 lig = 0;		//���C�g
	float3 toEye = normalize(eyePos - psIn.worldPos);		//�_���王�_�܂ł̐��K���x�N�g��
	float metaric = 0.0f;			//�X�y�L����
	float4 albedoColor = g_texture.Sample(g_sampler, psIn.uv);	//�e�N�X�`���J���[�B		
	if (hasSpecularMap) {
		//�X�y�L�����}�b�v������B
		metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	}
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

	//�ŏI�I�ȐF�����肷��B
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
