#pragma once

#include "ICarState.h"

/// <summary>
/// �Ԃ̃u���[�L���X�e�[�g
/// </summary>
class CarStateBrake : public ICarState
{
public:
	CarStateBrake(Car* car);
	~CarStateBrake();

	//�h���t�g�\���H
	bool IsDrift() override final
	{
		return true;
	}
protected:
	void Enter()override final;		//���O�����B
	void Leave()override final;		//���㏈���B
	void Execute()override final;	//���s�����B
private:
	prefab::CSoundSource* m_breakeSound = nullptr;
};

