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
	/// ���͂��X�V���鏈��
	/// </summary>
	void Update()override final;
	/// <summary>
	/// ���삷��Ԃ�ݒ肷��B
	/// </summary>
	/// <param name="car">�ԃN���X�B</param>
	void SetCar(Car* car)
	{
		m_car = car;
	}
private:
	float LinearInterpolation(float start, float end, float linear);

private:
	Car* m_car = nullptr;
};

