#pragma once

#include "ICarState.h"

/// <summary>
/// 車のブレーキ時ステート
/// </summary>
class CarStateBrake : public ICarState
{
public:
	CarStateBrake(CarMoveController* car);
	~CarStateBrake();

	//ドリフト可能か？
	bool IsDrift() override final
	{
		return false;
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

