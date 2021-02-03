#pragma once

#include "ICarState.h"

/// <summary>
/// 車のブレーキ時ステート
/// </summary>
class CarStateBrake : public ICarState
{
public:
	CarStateBrake(Car* car);
	~CarStateBrake();
protected:
	void Enter()override final;		//事前処理。
	void Leave()override final;		//事後処理。
	void Execute()override final;	//実行処理。
};

