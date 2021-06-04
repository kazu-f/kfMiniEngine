#pragma once

#include "Car/Car.h"
class CarStateIdle;
class CarStateAccele;

/// <summary>
/// 車のステートクラス。
/// </summary>
class ICarState
{
public:
	ICarState(Car* car);			//コンストラクタ。
	virtual ~ICarState();			//デストラクタ。

	virtual bool IsDrift() = 0;		//ドリフトが可能か？
	virtual bool IsDecelerate() = 0;	//減速中か？

	friend class Car;
protected:
	virtual void Enter() = 0;		//事前処理。
	virtual void Leave() = 0;		//事後処理。
	virtual void Execute() = 0;		//実行処理。

protected:
	Car* m_car = nullptr;			//車クラス。
};

