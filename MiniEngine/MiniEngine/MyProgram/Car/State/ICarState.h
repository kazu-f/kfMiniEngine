#pragma once

#include "Car/CarMoveController.h"
class CarStateIdle;
class CarStateAccele;

/// <summary>
/// �Ԃ̃X�e�[�g�N���X�B
/// </summary>
class ICarState
{
public:
	ICarState(CarMoveController* car);			//�R���X�g���N�^�B
	virtual ~ICarState();			//�f�X�g���N�^�B

	virtual bool IsDrift() = 0;		//�h���t�g���\���H
	virtual bool IsDecelerate() = 0;	//���������H

public:
	virtual void Enter() = 0;		//���O�����B
	virtual void Leave() = 0;		//���㏈���B
	virtual void Execute() = 0;		//���s�����B

protected:
	CarMoveController* m_car = nullptr;			//�ԃN���X�B
};

