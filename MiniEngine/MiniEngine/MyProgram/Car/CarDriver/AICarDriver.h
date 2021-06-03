#pragma once
#include "ICarDriver.h"

class Car;

class AICarDriver : public ICarDriver
{
public:
	AICarDriver(Car* car) :
		m_car(car) {};
	~AICarDriver() {};
	/// <summary>
	/// 入力を更新する処理
	/// </summary>
	void Update()override final;
	/// <summary>
	/// 操作する車を設定する。
	/// </summary>
	/// <param name="car">車クラス。</param>
	void SetCar(Car* car)
	{
		m_car = car;
	}
private:
	float LinearInterpolation(float start, float end, float linear);

private:
	Car* m_car = nullptr;
};

