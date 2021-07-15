#pragma once
#include "ICarState.h"
class CarStateDrift : public ICarState
{
public:
	CarStateDrift(CarMoveController* car);
	~CarStateDrift();

	//�h���t�g�\���H
	bool IsDrift() override final
	{
		return true;
	}
	//���������H
	bool IsDecelerate() override final
	{
		return true;
	}
protected:
	void Enter()override final;		//���O�����B
	void Leave()override final;		//���㏈���B
	void Execute()override final;	//���s�����B
private:
};

