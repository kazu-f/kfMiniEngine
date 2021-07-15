#pragma once

#include "ICarState.h"

/// <summary>
/// アクセルステート。
/// </summary>
class CarStateAccele : public ICarState
{
public:
	CarStateAccele(CarMoveController* car);		//コンストラクタ。
	~CarStateAccele();				//デストラクタ。

	//ドリフト可能か？
	bool IsDrift() override final
	{
		return false;
	}
	//減速中か？
	bool IsDecelerate() override final
	{
		return false;
	}

protected:
	void Enter()override final;		//事前処理。
	void Leave()override final;		//事後処理。
	void Execute()override final;	//実行処理。
private:
};

