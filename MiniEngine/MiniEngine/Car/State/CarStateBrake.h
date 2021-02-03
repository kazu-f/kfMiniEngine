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
protected:
	void Enter()override final;		//���O�����B
	void Leave()override final;		//���㏈���B
	void Execute()override final;	//���s�����B
};

