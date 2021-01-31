#include "stdafx.h"
#include "ICarState.h"

ICarState::ICarState(Car* car):
	m_car(car)
{
}

ICarState::~ICarState()
{
}
