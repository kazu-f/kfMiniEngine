#include "stdafx.h"
#include "DebugScene.h"
#include "Player/Player.h"
#include "Player/PlayerBall.h"
#include "GameLight/SceneLight.h"
#include "GameCamera/GameCamera.h"
#include "Car/Car.h"

namespace {

	const wchar_t* BGM_FILEPATH = L"Assets/sound/wanage.wav";

	const wchar_t* SE_FILEPATH = L"Assets/sound/SE_Katana.wav";

	const char* SPRITE_FILEPATH = "Assets/sprite/LAP.dds";
	const Vector2 SPRITE_SIZE = { 600.0f, 300.0f };
	const Vector2 SPRITE_PIVOT = { 0.0f,0.0f };
	const bool SPRITE_IS3D = false;
}

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

	//{
	//	ModelInitData modelData;
	//	modelData.m_tkmFilePath = "Assets/modelData/tree/AlaskaCedar_1.tkm";

	//	auto tree = NewGO<prefab::ModelRender>(0);
	//	tree->Init(modelData);
	//	tree->SetForwardRenderFlag(true);
	//	//tree->SetScale({ 0.05f,0.05f,0.05f });
	//	tree->SetShadowReceiverFlag(true);
	//	tree->SetShadowCasterFlag(true);
	//}

	m_courseLevel.Init("Assets/level/testLevel.tkl", [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"unityChan") == 0) {

			//m_player = NewGO<Player>(0);
			m_ball = NewGO<PlayerBall>(0);

			return true;
		}
		//if (objData.EqualObjectName(L"testbg/testBox")) {
		//	return true;
		//}

		return false;
		});

	PhysicsWorld().SetDebugMode(btIDebugDraw::DBG_DrawWireframe);

	return true;
}

void DebugScene::OnDestroy()
{
	DeleteGO(m_bgm);
	DeleteGO(m_sprite);
	DeleteGO(m_light);
	DeleteGO(m_player);

}

void DebugScene::Update()
{
	if (Pad(0).IsTrigger(enButtonX)) {
		if (m_bgm != nullptr) {
			DeleteGO(m_bgm);
			m_bgm = nullptr;
		}
	}
	if (Pad(0).IsTrigger(enButtonY)) {
		OnBGM();
	}
	if (Pad(0).IsTrigger(enButtonA)) {
		PlaySE();
	}
}

void DebugScene::OnBGM()
{
	if (m_bgm != nullptr) return;
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->InitStreaming(BGM_FILEPATH);
	m_bgm->Play(true);
}

void DebugScene::PlaySE()
{
	auto se = NewGO<prefab::CSoundSource>(0);
	se->Init(SE_FILEPATH);
	se->Play(false);
}

void DebugScene::InitSprite()
{
	//スプライトを表示する。
	m_sprite = NewGO<prefab::CSpriteRender>(0);
	SpriteInitData spriteData;
	spriteData.m_ddsFilePath[0] = SPRITE_FILEPATH;
	spriteData.m_height = static_cast<UINT>(SPRITE_SIZE.y);
	spriteData.m_width = static_cast<UINT>(SPRITE_SIZE.x);
	spriteData.m_fxFilePath = Sprite::SPRITE_SHADER_PATH;
	m_sprite->Init(spriteData, SPRITE_IS3D);
	m_sprite->SetPivot(SPRITE_PIVOT);
}


