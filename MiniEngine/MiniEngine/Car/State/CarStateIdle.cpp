#include "stdafx.h"
#include "CarStateIdle.h"
#include "CarStateAccele.h"
#include "CarStateBrake.h"

namespace {
	const float DICCELATION = 30.0f;
}

CarStateIdle::CarStateIdle(Car* car):
	ICarState::ICarState(car)
{
	m_idleSound = NewGO<prefab::CSoundSource>(0);
	m_idleSound->Init(L"Assets/sound/Car/CarDeceleration.wav");
}

CarStateIdle::~CarStateIdle()
{
	//サウンドの削除。
	DeleteGO(m_idleSound);
}

void CarStateIdle::Enter()
{
	m_idleSound->Play(true);
}

void CarStateIdle::Leave()
{
	m_idleSound->Stop();
}

void CarStateIdle::Execute()
{
	//何もしてないときは少しずつ速度を落とす。
	m_car->AddDicceleration(DICCELATION);

	if (g_pad[0]->IsPress(enButtonA))
	{
		m_car->ChangeState(m_car->m_stateAccele.get());
	}
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_car->ChangeState(m_car->m_stateBrake.get());
	}
}
