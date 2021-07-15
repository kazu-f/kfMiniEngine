#include "stdafx.h"
#include "CarStateBrake.h"
#include "CarStateIdle.h"
#include "CarStateAccele.h"
#include "CarStateDrift.h"
#include "Car/CarDriver/ICarDriver.h"
#include "Car/CarMoveController.h"

namespace {
	const float BRAKE_POWER = 30.0f;
}

CarStateBrake::CarStateBrake(CarMoveController* car) :
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
	m_car->AddDicceleration(BRAKE_POWER);

	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverDrift))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateDrift);
	}
	else if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverAccele))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateAccele);
	}
	else if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverIdle))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateIdle);
	}

}
