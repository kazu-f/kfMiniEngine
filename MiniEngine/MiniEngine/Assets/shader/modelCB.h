

//���f���p�̒萔�o�b�t�@
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

/*
*	���C�g�p�̒萔�o�b�t�@
*	tkLightManager.h��SLightParam�ƑΉ�����B
*/
cbuffer lightCb : register(b1)
{
	float3 eyePos			: packoffset(c0);		//�J�����̈ʒu�B
	int numDirectionLight	: packoffset(c0.w);		//�f�B���N�V�������C�g�̐��B
	float3 ambientLight		: packoffset(c1);		//�A���r�G���g���C�g�B
	int numPointLight		: packoffset(c1.w);		//�|�C���g���C�g�̐��B
	float4 screenParam		: packoffset(c2);		//�X�N���[���p�����[�^(neae,far,screenW,screenH)
}

/*
*	�}�e���A���̒萔�o�b�t�@
*	Material.h��SMaterialParam�ɑΉ�����B
*/
cbuffer MaterialParamCb : register(b2) {
	int hasNormalMap;		//�@���}�b�v������H
	int hasSpecularMap;		//�X�y�L�����}�b�v������H
}

//�V���h�E�}�b�v�̐�
static const int NUM_SHADOW_MAP = 3;
/*
*	�V���h�E�}�b�v�p�̒萔�o�b�t�@�B
*/
cbuffer ShadowCb : register(b3) {
	float4x4 mLVP[NUM_SHADOW_MAP];		//���C�g�r���[�v���W�F�N�V�����s��B
	float4 texOffset[NUM_SHADOW_MAP];	//�V���h�E�}�b�v�̃T�C�Y�B
	float depthOffset[NUM_SHADOW_MAP];	//�[�x�I�t�Z�b�g�B�V���h�E�A�N�l(�`����)����̂��߂̃o�C�A�X�B
										//�l���傫���قǉe�������ɂ����Ȃ�B
	float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//�J������Ԃł̉e�𗎂Ƃ��G���A�̐[�x�e�[�u���B
}