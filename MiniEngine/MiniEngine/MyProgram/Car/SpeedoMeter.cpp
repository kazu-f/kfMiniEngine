#include "stdafx.h"
#include "SpeedoMeter.h"
#include "Car.h"

SpeedoMeter::SpeedoMeter(Car* car)
{
	m_car = car;
}

SpeedoMeter::~SpeedoMeter()
{
}

void SpeedoMeter::UpdateSpeed(float speed)
{
}
