#include "stdafx.h"
#include "CarStateAccele.h"

namespace {
	const float ACCELE_SPEED = 100.0f;
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
	m_car->CalcDirection();

	Vector3 accele = Vector3::Zero;
	accele += m_car->GetForward() * ACCELE_SPEED;
	m_car->AddAcceleration(accele);

}
