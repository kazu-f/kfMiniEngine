#include "stdafx.h"
#include "CarStateAccele.h"
#include "CarStateIdle.h"

namespace {
	const float FIRST_SPEED = 280.0f;
	const float ACCELE_SPEED = 80.0f;
}

CarStateAccele::CarStateAccele(Car* car):
	ICarState::ICarState(car)
{
}

CarStateAccele::~CarStateAccele()
{
}

void CarStateAccele::Enter()
{
	m_car->AddAcceleration(FIRST_SPEED);
}

void CarStateAccele::Leave()
{
}

void CarStateAccele::Execute()
{
	//const float PadX = g_pad[0]->GetLStickXF();
	//const float PadY = g_pad[0]->GetLStickYF();
	//const float DeltaTime = GameTime().GetFrameDeltaTime();

	m_car->AddAcceleration(ACCELE_SPEED);

	if (!g_pad[0]->IsPress(enButtonA))
	{
		m_car->ChangeState(m_car->m_stateIdle.get());
	}
}
