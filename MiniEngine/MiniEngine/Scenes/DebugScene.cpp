#include "stdafx.h"
#include "DebugScene.h"
#include "Player/Player.h"
#include "GameLight/SceneLight.h"
#include "GameCamera/GameCamera.h"

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

	m_level.Init("Assets/level/testLevel.tkl", [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"unityChan") == 0) {

			m_player = NewGO<Player>(0);

			return true;
		}

		return false;
		});

	return true;
}

void DebugScene::OnDestroy()
{
}

void DebugScene::Update()
{
}

