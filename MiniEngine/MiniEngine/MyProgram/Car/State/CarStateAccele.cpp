#include "stdafx.h"
#include "CarStateAccele.h"
#include "CarStateIdle.h"

namespace {
	const float FIRST_SPEED = 280.0f;
	const float ACCELE_SPEED = 80.0f;
}

CarStateAccele::CarStateAccele(Car* car):
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
	m_car->AddAcceleration(FIRST_SPEED);
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

	if (!Pad(0).IsPress(enButtonA))
	{
		m_car->ChangeState(m_car->m_stateIdle.get());
	}
}

void CarStateAccele::CalcSoundFrequencyRetio()
{
	float speed = m_car->GetSpeed();
	m_soundFrequency = speed * (1.0f / Car::MAX_SPEED);
	m_soundFrequency = min(1.4f,max(0.6f, m_soundFrequency));
	m_acceleSound->SetFrequencyRetio(m_soundFrequency);
}
