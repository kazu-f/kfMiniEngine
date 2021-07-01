#include "stdafx.h"
#include "CarStateAccele.h"
#include "CarStateIdle.h"
#include "CarStateDrift.h"
#include "CarStateBrake.h"
#include "Car/CarDriver/ICarDriver.h"
#include "Car/CarMoveController.h"

namespace {
	const float FIRST_SPEED = 120.0f;
	const float ACCELE_SPEED = 10.0f;
}

CarStateAccele::CarStateAccele(CarMoveController* car):
	ICarState::ICarState(car)
{
	m_acceleSound = NewGO<prefab::CSoundSource>(5);
	m_acceleSound->Init(L"Assets/sound/Car/CarAcceleration.wav");
}

CarStateAccele::~CarStateAccele()
{
	//サウンドの削除。
	DeleteGO(m_acceleSound);
}

void CarStateAccele::Enter()
{
	if (m_car->GetSpeed() < FIRST_SPEED) {
		m_car->AddAcceleration(FIRST_SPEED);
	}
	m_acceleSound->Play(true);
	CalcSoundFrequencyRetio();
}

void CarStateAccele::Leave()
{
	m_acceleSound->Stop();
}

void CarStateAccele::Execute()
{
	//const float PadX = g_pad[0]->GetLStickXF();
	//const float PadY = g_pad[0]->GetLStickYF();
	//const float DeltaTime = GameTime().GetFrameDeltaTime();

	m_car->AddAcceleration(ACCELE_SPEED);

	CalcSoundFrequencyRetio();

	if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverDrift))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateDrift);
	}
	else if (m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverBrake))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateBrake);
	}
	else if(m_car->GetCarDriver()->GetDriverInput(ICarDriver::enDriverIdle))
	{
		m_car->ChangeState(CarMoveController::EnCarState::enStateIdle);
	}
}

void CarStateAccele::CalcSoundFrequencyRetio()
{
	float speed = m_car->GetSpeed();
	m_soundFrequency = speed * (1.0f / CarMoveController::MAX_SPEED);
	m_soundFrequency = min(1.4f,max(0.6f, m_soundFrequency));
	m_acceleSound->SetFrequencyRetio(m_soundFrequency);
}
