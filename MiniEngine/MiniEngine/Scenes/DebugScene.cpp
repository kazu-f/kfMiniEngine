#include "stdafx.h"
#include "DebugScene.h"
#include "Player/Player.h"
#include "GameLight/SceneLight.h"
#include "GameCamera/GameCamera.h"
#include "Car/Car.h"

DebugScene::DebugScene()
{
}

DebugScene::~DebugScene()
{
}

bool DebugScene::Start()
{
	m_light = NewGO<SceneLight>(0);

	m_camera = NewGO<CGameCamera>(0);

	//NewGO<Car>(0);

	OnBGM();

	//InitSprite();

	m_courseLevel.Init("Assets/level/testLevel.tkl", [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"unityChan") == 0) {

			m_player = NewGO<Player>(0);

			return true;
		}

		return false;
		});

	PhysicsWorld().SetDebugMode(btIDebugDraw::DBG_DrawWireframe);

	return true;
}

void DebugScene::OnDestroy()
{
}

void DebugScene::OnBGM()
{
	if (m_bgm != nullptr) return;
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->InitStreaming(L"Assets/sound/wanage.wav");
	m_bgm->Play(true);
}

void DebugScene::PlaySE()
{
	auto se = NewGO<prefab::CSoundSource>(0);
	se->Init(L"Assets/sound/SE_Katana.wav");
	se->Play(false);
}

void DebugScene::InitSprite()
{
	//スプライトを表示する。
	m_sprite = NewGO<prefab::CSpriteRender>(0);
	SpriteInitData spriteData;
	spriteData.m_ddsFilePath[0] = "Assets/sprite/LAP.dds";
	spriteData.m_height = 300.0f;
	spriteData.m_width = 600.0f;
	spriteData.m_fxFilePath = Sprite::SPRITE_SHADER_PATH;
	m_sprite->Init(spriteData);
	m_sprite->SetPivot({ 0.0f,0.0f });
}

void DebugScene::Update()
{
	if (g_pad[0]->IsTrigger(enButtonX)) {
		if (m_bgm != nullptr) {
			DeleteGO(m_bgm);
			m_bgm = nullptr;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonY)) {
		OnBGM();
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		PlaySE();
	}
}

