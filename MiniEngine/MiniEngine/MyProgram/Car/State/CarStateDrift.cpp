#include "stdafx.h"
#include "CarStateDrift.h"
#include "CarStateIdle.h"
#include "CarStateBrake.h"
#include "CarStateAccele.h"

namespace {
	const float FIRST_DICCELATION = 120.0f;
	const float DICCELATION = 10.0f;
}

CarStateDrift::CarStateDrift(Car* car):
	ICarState::ICarState(car)
{
	m_breakeSound = NewGO<prefab::CSoundSource>(0);
	m_breakeSound->Init(L"Assets/sound/Car/CarSkid.wav");
}

CarStateDrift::~CarStateDrift()
{
	//サウンドの削除。
	DeleteGO(m_breakeSound);
}

void CarStateDrift::Enter()
{
	m_breakeSound->Play(true);
}

void CarStateDrift::Leave()
{
	m_breakeSound->Stop();
	m_driftSpeed = 0.0f;
}

void CarStateDrift::Execute()
{
	m_car->AddDicceleration(DICCELATION);

	if (m_car->m_carDriver->GetDriverInput(ICarDriver::enDriverBrake))
	{
		m_car->ChangeState(m_car->m_stateBrake.get());
	}
	else if (m_car->m_carDriver->GetDriverInput(ICarDriver::enDriverAccele))
	{
		m_car->ChangeState(m_car->m_stateAccele.get());
	}
	else if (m_car->m_carDriver->GetDriverInput(ICarDriver::enDriverIdle))
	{
		m_car->ChangeState(m_car->m_stateIdle.get());
	}
}
