#include "stdafx.h"
#include "CarStateIdle.h"
#include "CarStateAccele.h"
#include "CarStateBrake.h"
#include "Car/CarDriver/ICarDriver.h"
#include "Car/CarMoveController.h"

namespace {
	const float DICCELATION = 10.0f;
}

CarStateIdle::CarStateIdle(CarMoveController* car):
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
	m_car->AddDicceleration(DICCELATION);

	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverAccele))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateAccele);
	}
	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverBrake))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateBrake);
	}
}
