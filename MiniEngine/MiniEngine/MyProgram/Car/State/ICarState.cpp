#include "stdafx.h"
#include "ICarState.h"
#include "Car/CarMoveController.h"


ICarState::ICarState(CarMoveController* car):
	m_car(car)
{
}

ICarState::~ICarState()
{
}
