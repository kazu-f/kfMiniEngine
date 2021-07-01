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

	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverAccele))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateAccele);
	}
	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverBrake))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateBrake);
	}
}
