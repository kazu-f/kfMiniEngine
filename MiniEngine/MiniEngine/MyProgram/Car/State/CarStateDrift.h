#pragma once
#include "ICarState.h"
class CarStateDrift : public ICarState
{
public:
	CarStateDrift(CarMoveController* car);
	~CarStateDrift();

	//ドリフト可能か？
	bool IsDrift() override final
	{
		return true;
	}
	//減速中か？
	bool IsDecelerate() override final
	{
		return true;
	}
protected:
	void Enter()override final;		//事前処理。
	void Leave()override final;		//事後処理。
	void Execute()override final;	//実行処理。
private:
};

