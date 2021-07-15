#include "stdafx.h"
#include "CarSoundController.h"
#include "Car.h"
#include "CarMoveController.h"
#include "State/ICarState.h"
#include "State/CarStateIdle.h"
#include "State/CarStateAccele.h"
#include "State/CarStateBrake.h"
#include "State/CarStateDrift.h"

namespace {
	const wchar_t* ENGINE_SE_FILEPATH = L"Assets/sound/Car/CarAcceleration.wav";
	const wchar_t* BRAKE_SE_FILEPATH = L"Assets/sound/Car/CarSkid.wav";

	const float MIN_SOUND_RAITO = 0.5f;
	const float MAX_SOUND_RAITO = 1.4f;
	const float SOUND_RAITO_WAIGHT = 1.6f;
}

CarSoundController::CarSoundController()
{
}

CarSoundController::~CarSoundController()
{
}

void CarSoundController::Init()
{
	//�G���W���̉���ǂݍ��ށB
	m_engineSE = NewGO<prefab::CSoundSource>(0);
	m_engineSE->Init(ENGINE_SE_FILEPATH);
	//�u���[�L�̉���ǂݍ��ށB
	m_brakeSE = NewGO<prefab::CSoundSource>(0);
	m_brakeSE->Init(BRAKE_SE_FILEPATH);
}

void CarSoundController::UpdateCarSound(Car* car)
{
	if (!m_engineSE->IsPlaying()) {
		m_engineSE->Play(true);
	}

	PlayBrake(car);

	CalcSoundFrequencyRetio(car);
}

void CarSoundController::CalcSoundFrequencyRetio(Car* car)
{
	float speed = car->GetCarMoveController()->GetSpeed();
	m_soundFrequency = speed * (1.0f / CarMoveController::MAX_SPEED) * SOUND_RAITO_WAIGHT;
	m_soundFrequency = min(MAX_SOUND_RAITO, max(MIN_SOUND_RAITO, m_soundFrequency));
	m_engineSE->SetFrequencyRetio(m_soundFrequency);
}

void CarSoundController::PlayBrake(Car* car)
{
	if (car->GetCarMoveController()->GetCarState()->IsDecelerate()
		&& !m_brakeSE->IsPlaying()) {
		m_brakeSE->Play(true);
	}
	else if(!car->GetCarMoveController()->GetCarState()->IsDecelerate() &&
		m_brakeSE->IsPlaying()){
		m_brakeSE->Stop();
	}
}