#include "stdafx.h"
#include "CarStateAccele.h"
#include "CarStateIdle.h"

namespace {
	const float ACCELE_SPEED = 400.0f;
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
}

void CarStateAccele::Leave()
{
}

void CarStateAccele::Execute()
{
	const float PadX = g_pad[0]->GetLStickXF();
	const float PadY = g_pad[0]->GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	float accele = 0.0f;
	accele += ACCELE_SPEED * DeltaTime;
	m_car->AddAcceleration(accele);

	if (!g_pad[0]->IsPress(enButtonA))
	{
		m_car->ChangeState(m_car->m_stateIdle.get());
	}
}
