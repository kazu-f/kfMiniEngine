#include "stdafx.h"
#include "SoundTestScene.h"

namespace {
	const char* BALL_MODEL = "Assets/modelData/testbg/Ball.tkm";		//モデルのファイルパス。
	const char* GROUND_MODEL = "Assets/modelData/testbg/bg.tkm";		//床モデルのファイルパス。
	const wchar_t* ENGINE_SE_FILEPATH = L"Assets/sound/Car/CarAcceleration.wav";
	const wchar_t* RACE_BGM = L"Assets/sound/Title/RaceGameTitle.wav";

	const Vector3 CAMERA_POS = { 0.0f,50.0f,500.0f };
	const float MOVESPEED = 200.0f;
}

SoundTestScene::SoundTestScene()
{
}

SoundTestScene::~SoundTestScene()
{
}

bool SoundTestScene::Start()
{
	//モデル。
	ModelInitData modelData;
	//床。
	modelData.m_tkmFilePath = GROUND_MODEL;
	m_ground = NewGO<prefab::ModelRender>(3);
	m_ground->Init(modelData);
	//球。
	modelData.m_tkmFilePath = BALL_MODEL;
	m_model = NewGO<prefab::ModelRender>(3);
	m_model->Init(modelData);
	m_model->SetPosition(m_soundPos);

	//サウンド。
	m_sound = NewGO<prefab::CSoundSource>(3);
	m_sound->Init(ENGINE_SE_FILEPATH, true);
	//m_sound->InitStreaming(RACE_BGM, true);
	m_sound->SetPosition(m_soundPos);
	m_sound->Play(true);
	m_sound->SetCurveDistanceScaler(800.0f);

	m_camPos = CAMERA_POS;

	MainCamera().SetPosition(m_camPos);
	MainCamera().SetTarget(m_soundPos);

	CFade::GetInstance()->StartFadeIn();

	return true;
}

void SoundTestScene::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGO(m_ground);
	DeleteGO(m_sound);
}

void SoundTestScene::Update()
{
	if (Pad(0).IsTrigger(enButtonStart))
	{
		m_moveFlag ^= 1;
	}

	if (m_moveFlag) {
		MoveCamera();
	}
	else {
		MoveSound();
	}

	m_pitch += Pad(0).GetRStickYF() * GameTime().GetFrameDeltaTime();
	m_pitch = min(2.0f, max(0.5f, m_pitch));
	m_sound->SetFrequencyRetio(m_pitch);

	MainCamera().SetTarget(m_soundPos);
	MainCamera().SetPosition(m_camPos);
	m_sound->SetPosition(m_soundPos);
	m_model->SetPosition(m_soundPos);

	GameEngine().GetSoundEngine().SetListenerPosition(m_camPos);
}

void SoundTestScene::MoveCamera()
{
	const float DelTime = GameTime().GetFrameDeltaTime();
	const float PadX = Pad(0).GetLStickXF() * DelTime;
	const float PadY = Pad(0).GetLStickYF() * DelTime;

	Vector3 right = MainCamera().GetRight();
	right.y = 0.0f;
	right.Normalize();
	Vector3 forward = MainCamera().GetForward();
	forward.y = 0.0f;
	forward.Normalize();

	m_camPos += right * PadX * MOVESPEED;
	m_camPos += forward * PadY * MOVESPEED;

}

void SoundTestScene::MoveSound()
{
	const float DelTime = GameTime().GetFrameDeltaTime();
	const float PadX = Pad(0).GetLStickXF() * DelTime;
	const float PadY = Pad(0).GetLStickYF() * DelTime;

	Vector3 right = MainCamera().GetRight();
	right.y = 0.0f;
	right.Normalize();
	Vector3 forward = MainCamera().GetForward();
	forward.y = 0.0f;
	forward.Normalize();

	m_soundPos += right * PadX * MOVESPEED;
	m_soundPos += forward * PadY * MOVESPEED;
}


