#pragma once
#include "ICarState.h"
class CarStateIdle : public ICarState
{
public:
	CarStateIdle(Car* car);			//コンストラクタ。
	~CarStateIdle();				//デストラクタ。
protected:
	void Enter()override final;		//事前処理。
	void Leave()override final;		//事後処理。
	void Execute()override final;	//実行処理。
};

