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
	//�T�E���h�̍폜�B
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
	//�������ĂȂ��Ƃ��͏��������x�𗎂Ƃ��B
	m_car->AddDicceleration(DICCELATION);

	if (m_car->m_carDriver->GetDriverInput(ICarDriver::enDriverAccele))
	{
		m_car->ChangeState(m_car->m_stateAccele.get());
	}
	if (m_car->m_carDriver->GetDriverInput(ICarDriver::enDriverBrake))
	{
		m_car->ChangeState(m_car->m_stateBrake.get());
	}
}
