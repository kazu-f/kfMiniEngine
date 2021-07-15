#include "stdafx.h"
#include "CarStateDrift.h"
#include "CarStateIdle.h"
#include "CarStateBrake.h"
#include "CarStateAccele.h"
#include "Car/CarDriver/ICarDriver.h"
#include "Car/CarMoveController.h"

namespace {
	const float FIRST_DICCELATION = 120.0f;
	const float DICCELATION = 15.0f;
}

CarStateDrift::CarStateDrift(CarMoveController* car):
	ICarState::ICarState(car)
{
}

CarStateDrift::~CarStateDrift()
{
}

void CarStateDrift::Enter()
{
}

void CarStateDrift::Leave()
{
}

void CarStateDrift::Execute()
{
	m_car->AddDicceleration(DICCELATION);

	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverBrake))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateBrake);
	}
	else if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverAccele))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateAccele);
	}
	else if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverIdle))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateAccele);
	}
}
