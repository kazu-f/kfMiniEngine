/*
*		���f���V�F�[�_�[�B
*/

#include "modelCB.h"
#include "modelStruct.h"
#include "PBR.h"

//���f���e�N�X�`���B
Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
//�{�[���s��B
StructuredBuffer<float4x4> boneMatrix : register(t3);
//�f�B���N�V�������C�g�B
StructuredBuffer<SDirectionalLight> directionalLight : register(t4);

Texture2D<float4> shadowMap_0 : register(t5);		//�V���h�E�}�b�v�B
Texture2D<float4> shadowMap_1 : register(t6);		//�V���h�E�}�b�v�B
Texture2D<float4> shadowMap_2 : register(t7);		//�V���h�E�}�b�v�B

StructuredBuffer<float4x4> instancingDatas : register(t8);		//�C���X�^���V���O�`��p�̃f�[�^�B
//�T���v���X�e�[�g�B
sampler g_sampler : register(s0);

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

/*
*	�e�������Ă��邩���v�Z����B
*/
float CalcShadowPercent(Texture2D<float4> tex, float2 uv, float2 offset, float depth, float dOffset)
{
	float shadow_val = tex.Sample(g_sampler, uv).r;
	if (depth > shadow_val + dOffset) {
		return 1.0f;
	}
	return 0.0f;
}

/*
*	�J�X�P�[�h�C���f�b�N�X�̎擾�B
*/
int GetCascadeIndex(float zInView)
{
	[unroll]
	for (int i = 0; i < NUM_SHADOW_MAP; i++) {
		if (zInView < shadowAreaDepthInViewSpace[i]) {
			return i;
		}
	}
	return -1;
}

/*
*	�J�X�P�[�h�V���h�E�̏����B
*/
float CalcShadow(float3 worldPos, float zInView)
{
	float shadow = 0.0f;
	if (isShadowReceiver) {
		//�e�𗎂Ƃ��B
		//�g�p����V���h�E�}�b�v�ԍ��̎擾�B
		int cascadeIndex = GetCascadeIndex(zInView);
		if (cascadeIndex == -1) {
			return shadow;
		}

		float4 posInLVP = mul(mLVP[cascadeIndex], float4(worldPos, 1.0f));
		posInLVP.xyz /= posInLVP.w;

		float depth = min(posInLVP.z, 1.0f);

		//uv���W�ϊ��B
		float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);

		if (cascadeIndex == 0) {
			shadow = CalcShadowPercent(shadowMap_0, shadowMapUV, texOffset[cascadeIndex], depth, depthOffset.x);
		}
		if (cascadeIndex == 1) {
			shadow = CalcShadowPercent(shadowMap_1, shadowMapUV, texOffset[cascadeIndex], depth, depthOffset.y);
		}
		if (cascadeIndex == 2) {
			shadow = CalcShadowPercent(shadowMap_2, shadowMapUV, texOffset[cascadeIndex], depth, depthOffset.z);
		}
	}
	return shadow;
}

//�@�����v�Z�B
float3 CalcNormal(float3 normal, float3 biNormal, float3 tangent, float2 uv)
{
	//�@���̌v�Z�B
	float3 ret;
	if (hasNormalMap) {
		//�@���}�b�v����@�������������Ă���B
		float3 binSpaceNormal = g_normalMap.Sample(g_sampler, uv).xyz;
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;	//-1.0f�`1.0f�ɒ����B
		ret = tangent * binSpaceNormal.x		//�ڐ�(�@���ɑ΂��ĉE�H)
			+ biNormal * binSpaceNormal.y			//�]�@��(�@���ɑ΂��ď�)
			+ normal * binSpaceNormal.z;			//�@������
	}
	else {
		ret = normal;
	}
	return ret;
}

float3 PhongSpec(float3 L,float3 Normal,float3 toEye, float3 Color,float specPow)
{
	//���߂�toEyeDir�̔��˃x�N�g�������߂�B
	float3 reflectEyeDir = -toEye + 2 * dot(Normal, toEye) * Normal;
	//���߂����˃x�N�g���ƃf�B���N�V�������C�g�̕����Ƃ̓��ς�����āA�X�y�L�����̋������v�Z����B
	float3 t = max(0.0f, dot(-L, reflectEyeDir));
	//pow�֐����g���āA�X�y�L�������i��B�i��̋����͒萔�o�b�t�@�œn����Ă���B
	float3 spec = pow(t, specPow) * Color;
	//�D �X�y�L�������˂����܂�����Alig�ɉ��Z����B
	//���ʔ��˂𔽎ˌ��ɉ��Z����B
	return spec;
}

//���_�V�F�[�_�[�̃R�A�֐��B
SPSIn VSMainCore(SVSIn vsIn, float4x4 wMat)
{
	SPSIn psIn;

	psIn.pos = mul(wMat, vsIn.pos);						//���f���̒��_�����[���h���W�n�ɕϊ��B
	psIn.worldPos = psIn.pos.xyz;
	psIn.pos = mul(mView, psIn.pos);						//���[���h���W�n����J�������W�n�ɕϊ��B
	psIn.pos = mul(mProj, psIn.pos);						//�J�������W�n����X�N���[�����W�n�ɕϊ��B
	psIn.normal = normalize(mul(wMat, vsIn.normal));		//�@�������[���h���W�n�ɕϊ��B
	psIn.Tangent = normalize(mul(wMat, vsIn.Tangent));
	psIn.biNormal = normalize(mul(wMat, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// �X�L���������f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, mWorld);
}
/// <summary>
/// �X�L�������C���X�^�����f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
/// </summary>
SPSIn VSMainInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
	return VSMainCore(vsIn, instancingDatas[instanceID]);
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
SPSIn VSMainSkin(SVSIn vsIn)
{
	
	SPSIn psIn;

	float4x4 m;
	m = CalcSkinMatrix(vsIn.skinVert);

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

/*
	�X�L������C���X�^���V���O���f���p�̒��_�V�F�[�_�[�B
*/
SPSIn VSMainSkinInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
	SPSIn psIn;

	float4x4 m;
	m = CalcSkinMatrix(vsIn.skinVert);
	m = mul(instancingDatas[instanceID], m);

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

//�ʏ�̃��C�e�B���O�̃s�N�Z���V�F�[�_�[�B
float4 PSMain(SPSIn psIn) : SV_Target0
{

	float3 lig = 0;		//���C�g
	float4 albedoColor = g_texture.Sample(g_sampler, psIn.uv);	//�e�N�X�`���J���[�B		
	//�@���̌v�Z�B
	float3 normal = CalcNormal(psIn.normal,psIn.biNormal,psIn.Tangent,psIn.uv);
	//float3 normal = psIn.normal;
	float3 toEye = normalize(eyePos - psIn.worldPos);		//�_���王�_�܂ł̐��K���x�N�g��
	float metaric = 0.0f;			//�X�y�L����
	if (hasSpecularMap) {
		//�X�y�L�����}�b�v������B
		metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	}
	float roughness = 1.0f;			//�g�U���˂̖ʂ̑e���B
	for (int ligNo = 0; ligNo < numDirectionLight; ligNo++)
	{
		//�f�B�t�@�[�h�g�U���˂̐F�B
		float3 baseColor = max(dot(normal, -directionalLight[ligNo].direction), 0.0f) * directionalLight[ligNo].color.xyz;
		//DisneyModel�g�U����
		float disneyDiffuse = NormalizedDisneyDiffuse(normal, -directionalLight[ligNo].direction, toEye, roughness);
		float3 diffuse = baseColor * disneyDiffuse;

		////�ʏ�̐��K���g�U����
		//float3 diffuse = baseColor / PI;

		////�����̃����o�[�g�g�U���ˁB
		//float3 diffuse = baseColor;


		////�N�b�N�g�����X���f���̋��ʔ���
		float3 specCol = CookTrranceSpecular(-directionalLight[ligNo].direction, toEye, normal, metaric) * directionalLight[ligNo].color.xyz;

		//�t�H�����ʔ���
		//float3 specCol = PhongSpec(directionalLight[ligNo].direction, normal, toEye, directionalLight[ligNo].color.xyz, 1.3f);


		////�g�U���ˌ��Ƌ��ʔ��ˌ�����`�⊮�B
		lig += lerp(diffuse, specCol, metaric);

		//lig += specCol + diffuse;

	}
	//�����B
	lig += ambientLight / PI * (1.0f - metaric);
	//�V���h�E�B
	float4 posInView = mul(mView, float4(psIn.worldPos,1.0f));
	float shadow = CalcShadow(psIn.worldPos, posInView.z);

	lig *= lerp(1.0f, 0.5f, shadow);

	//�ŏI�I�ȐF�����肷��B
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	finalColor.a = albedoColor.a;
	return finalColor;
}

//GBuffer�ɏ������ރs�N�Z���V�F�[�_�[�̃G���g���֐��B
PSOut_GBuffer PSMain_RenderGBuffer (SPSIn psIn){
	PSOut_GBuffer Out = (PSOut_GBuffer)0;

	Out.albedo = g_texture.Sample(g_sampler, psIn.uv);		//�A���x�h�B
	//�@���}�b�v�B
	//float3 normal = CalcNormal(psIn.normal, psIn.biNormal, psIn.Tangent, psIn.uv);
	float3 normal = psIn.normal;
	Out.normal.xyz = (normal / 2.0f) + 0.5f;

	//���[���h���W�B
	Out.worldPos = float4(psIn.worldPos, 0.0f);

	//�X�y�L�����}�b�v�B
	Out.spec = 0.0f;
	//if (hasSpecularMap) {
	//	//�X�y�L�����}�b�v������B
	//	Out.spec = g_specularMap.Sample(g_sampler, psIn.uv).r;
	//}

	//�V���h�E�B
	float4 posInView = mul(mView, float4(psIn.worldPos, 1.0f));
	Out.shadow = CalcShadow(psIn.worldPos, posInView.z);

	return Out;
}


/*
*	�X�L���Ȃ����f���̃V���h�E�}�b�v�������ݗp�̒��_�V�F�[�_�[�B
*/
SShadowMapPSIn VSMainNonSkinShadowMap(SShadowMapVSIn vsIn)
{
	SShadowMapPSIn psIn;
	psIn.pos = mul(mWorld, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	return psIn;
}

/*
*	�X�L���Ȃ��C���X�^���V���O�`�惂�f���̃V���h�E�}�b�v�������ݗp�̒��_�V�F�[�_�[�B
*/
SShadowMapPSIn VSMainNonSkinInstancingShadowMap(SShadowMapVSIn vsIn, uint instanceID : SV_InstanceID)
{
	SShadowMapPSIn psIn;
	psIn.pos = mul(instancingDatas[instanceID], vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	return psIn;
}
/*
*	�X�L�����胂�f���̃V���h�E�}�b�v�������ݗp�̒��_�V�F�[�_�[�B
*/
SShadowMapPSIn VSMainSkinShadowMap(SShadowMapVSIn vsIn)
{
	SShadowMapPSIn psIn;
	float4x4 skinMatrix = CalcSkinMatrix(vsIn.skinVert);
	psIn.pos = mul(skinMatrix, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	return psIn;
}
/*
*	�X�L������C���X�^���V���O�`�惂�f���̃V���h�E�}�b�v�������ݗp�̒��_�V�F�[�_�[�B
*/
SShadowMapPSIn VSMainSkinInstancingShadowMap(SShadowMapVSIn vsIn, uint instanceID : SV_InstanceID)
{
	SShadowMapPSIn psIn;
	float4x4 skinMatrix = CalcSkinMatrix(vsIn.skinVert);
	float4x4 m = instancingDatas[instanceID];
	m = mul(m, skinMatrix);

	psIn.pos = mul(m, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	return psIn;
}
/*
*	�V���h�E�}�b�v�������ݗp�̃s�N�Z���V�F�[�_�[�B
*/
float4 PSMainShadowMap(SShadowMapPSIn psIn) :SV_Target0
{
	return psIn.pos.z / psIn.pos.w;
}