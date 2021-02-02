#include "stdafx.h"
#include "ICarState.h"
#include "CarStateIdle.h"
#include "CarStateAccele.h"


ICarState::ICarState(Car* car):
	m_car(car)
{
}

ICarState::~ICarState()
{
}
