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

	//�h���t�g�\���H
	bool IsDrift() override final
	{
		return false;
	}
	//���������H
	bool IsDecelerate() override final
	{
		return false;
	}

protected:
	void Enter()override final;		//���O�����B
	void Leave()override final;		//���㏈���B
	void Execute()override final;	//���s�����B
private:
	void CalcSoundFrequencyRetio();			//���ʉ��̍�����ݒ�B
private:
	prefab::CSoundSource* m_acceleSound = nullptr;
	float m_soundFrequency = 0.0f;
};

