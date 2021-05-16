/*
*	�X�J�C�L���[�u�V�F�[�_�[�B
*/

#include "modelCB.h"

//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn_SkyCube {
	float4 pos : POSITION;		//���f���̒��_���W�B
	float3 normal : NORMAL;		//�@���B
}

struct SPSIn_SkyCube {
	float4 pos : SV_POSITION;	//�X�N���[����Ԃł̃s�N�Z���̍��W�B
	float3 normal : NORMAL;		//�@���B
};

//��p�̒��_�V�F�[�_�[�B
SPSIn_SkyCube VSMain_SkyCube(SVSIn_SkyCube In) {
	SPSIn_SkyCube psIn;

	psIn.pos = mul(mWorld, vsIn.pos);						//���f���̒��_�����[���h���W�n�ɕϊ��B
	psIn.pos = mul(mView, psIn.pos);						//���[���h���W�n����J�������W�n�ɕϊ��B
	psIn.pos = mul(mProj, psIn.pos);						//�J�������W�n����X�N���[�����W�n�ɕϊ��B
	psIn.nomal = In.nomal;								//�@���B

	return psIn;
}

//��p�̃s�N�Z���V�F�[�_�[�B
float4 PSMain_SkyCube(SPSIn_SkyCube In) : SV_Target0
{
	float4 color = skyCubeMap.Sample(Sampler, In.Normal);
	return color;
}
