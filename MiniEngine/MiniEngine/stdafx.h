#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"

const int NUM_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̐��B

/// <summary>
/// �f�B���N�V�������C�g�B
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//���C�g�̕����B
	float pad0;			//�p�f�B���O�B
	Vector4 color;		//���C�g�̃J���[�B
};
/// <summary>
/// ���C�g�\���́B
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//�f�B���N�V�������C�g�B
	Vector3 eyePos;					//�J�����̈ʒu�B
	float specPow;					//�X�y�L�����̍i��B
	Vector3 ambinetLight;			//�����B
};