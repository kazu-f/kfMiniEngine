#include "stdafx.h"
#include "CarStateBrake.h"
#include "CarStateIdle.h"

namespace {
	const float BRAKE_POWER = 120.0f;
}

CarStateBrake::CarStateBrake(Car* car) :
	ICarState::ICarState(car)
{
}

CarStateBrake::~CarStateBrake()
{
}

void CarStateBrake::Enter()
{
}

void CarStateBrake::Leave()
{
}

void CarStateBrake::Execute()
{
	//ブレーキで減速する。
	m_car->AddDicceleration(BRAKE_POWER * 60.0f * GameTime().GetFrameDeltaTime());

	if (!g_pad[0]->IsPress(enButtonB))
	{
		m_car->ChangeState(m_car->m_stateIdle.get());
	}

}
