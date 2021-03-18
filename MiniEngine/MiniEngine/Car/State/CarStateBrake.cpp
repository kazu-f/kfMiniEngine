#include "stdafx.h"
#include "CarStateBrake.h"
#include "CarStateIdle.h"

namespace {
	const float BRAKE_POWER = 120.0f;
}

CarStateBrake::CarStateBrake(Car* car) :
	ICarState::ICarState(car)
{
	m_breakeSound = NewGO<prefab::CSoundSource>(0);
	m_breakeSound->Init(L"Assets/sound/Car/CarSkid.wav");
}

CarStateBrake::~CarStateBrake()
{
	//サウンドの削除。
	DeleteGO(m_breakeSound);
}

void CarStateBrake::Enter()
{
	m_breakeSound->Play(true);
}

void CarStateBrake::Leave()
{
	m_breakeSound->Stop();
}

void CarStateBrake::Execute()
{
	//ブレーキで減速する。
	m_car->AddDicceleration(BRAKE_POWER);

	if (!Pad(0).IsPress(enButtonB))
	{
		m_car->ChangeState(m_car->m_stateIdle.get());
	}

}
