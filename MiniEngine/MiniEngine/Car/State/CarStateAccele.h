#pragma once

#include "ICarState.h"

/// <summary>
/// �A�N�Z���X�e�[�g�B
/// </summary>
class CarStateAccele : public ICarState
{
public:
	CarStateAccele(Car* car);		//�R���X�g���N�^�B
	~CarStateAccele();				//�f�X�g���N�^�B

protected:
	void Enter()override final;		//���O�����B
	void Leave()override final;		//���㏈���B
	void Execute()override final;	//���s�����B
private:
	prefab::CSoundSource* m_acceleSound = nullptr;
};

