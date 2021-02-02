#include "stdafx.h"
#include "CarStateIdle.h"
#include "CarStateAccele.h"

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

	if (g_pad[0]->IsPress(enButtonA))
	{
		m_car->ChangeState(m_car->m_stateAccele.get());
	}
}
