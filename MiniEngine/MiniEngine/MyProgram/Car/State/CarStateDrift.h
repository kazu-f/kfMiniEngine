#pragma once
#include "ICarState.h"
class CarStateDrift : public ICarState
{
public:
	CarStateDrift(Car* car);
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
	prefab::CSoundSource* m_breakeSound = nullptr;
	float m_driftSpeed = 0.0f;
};

