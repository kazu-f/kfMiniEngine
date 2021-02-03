#include "stdafx.h"
#include "CarStateIdle.h"
#include "CarStateAccele.h"

namespace {
	const float DICCELATION = 30.0f;
}

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
	//‰½‚à‚µ‚Ä‚È‚¢‚Æ‚«‚Í­‚µ‚¸‚Â‘¬“x‚ð—Ž‚Æ‚·B
	m_car->AddDicceleration(DICCELATION * 60.0f * GameTime().GetFrameDeltaTime());

	if (g_pad[0]->IsPress(enButtonA))
	{
		m_car->ChangeState(m_car->m_stateAccele.get());
	}
}
