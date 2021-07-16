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

	const float SOUND_DISTANCE_SCALER = 28000.0f;
}

CarSoundController::CarSoundController()
{
}

CarSoundController::~CarSoundController()
{
}

void CarSoundController::Init()
{
	//エンジンの音を読み込む。
	m_engineSE = NewGO<prefab::CSoundSource>(0);
	m_engineSE->Init(ENGINE_SE_FILEPATH, true);
	m_engineSE->SetCurveDistanceScaler(SOUND_DISTANCE_SCALER);
	//ブレーキの音を読み込む。
	m_brakeSE = NewGO<prefab::CSoundSource>(0);
	m_brakeSE->Init(BRAKE_SE_FILEPATH, true);
	m_brakeSE->SetCurveDistanceScaler(SOUND_DISTANCE_SCALER);
}

void CarSoundController::Release()
{
	if (m_engineSE != nullptr)
	{
		DeleteGO(m_engineSE);
		m_engineSE = nullptr;
	}
	if (m_brakeSE != nullptr)
	{
		DeleteGO(m_brakeSE);
		m_brakeSE = nullptr;
	}
}

void CarSoundController::UpdateCarSound(Car* car)
{
	if (!m_engineSE->IsPlaying()) {
		m_engineSE->Play(true);
	}
	m_engineSE->SetVolume(m_volume);
	m_engineSE->SetPosition(car->GetPosition());
	m_brakeSE->SetVolume(m_volume);
	m_brakeSE->SetPosition(car->GetPosition());

	PlayBrake(car);

	CalcSoundFrequencyRetio(car);
}

void CarSoundController::CalcSoundFrequencyRetio(Car* car)
{
	float speedWeight = car->GetCarMoveController()->GetSpeed();
	speedWeight /= CarMoveController::MAX_SPEED;
	speedWeight = min(1.0f, max(0.0f, speedWeight));

	m_soundFrequency = MAX_SOUND_RAITO * speedWeight + MIN_SOUND_RAITO * (1.0f - speedWeight);

	//float speed = car->GetCarMoveController()->GetSpeed();
	//m_soundFrequency = speed * (1.0f / CarMoveController::MAX_SPEED) * SOUND_RAITO_WAIGHT;
	//m_soundFrequency = min(MAX_SOUND_RAITO, max(MIN_SOUND_RAITO, m_soundFrequency));
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
