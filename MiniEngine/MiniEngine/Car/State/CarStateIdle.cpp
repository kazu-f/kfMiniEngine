#include "stdafx.h"
#include "CarStateIdle.h"

CarStateIdle::CarStateIdle(Car* car):
	ICarState::ICarState(car)
{
}

CarStateIdle::~CarStateIdle()
{
}

void CarStateIdle::Enter()
{
}

void CarStateIdle::Leave()
{
}

void CarStateIdle::Execute()
{
}
