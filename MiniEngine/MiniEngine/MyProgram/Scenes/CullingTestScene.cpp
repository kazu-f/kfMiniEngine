#include "stdafx.h"
#include "CullingTestScene.h"
#include "Spectator/Spectator.h"
#include "Spectator/SpectatorNames.h"
#include "GameCamera/GameCamera.h"

CullingTestScene::CullingTestScene()
{
}

CullingTestScene::~CullingTestScene()
{
}

bool CullingTestScene::Start()
{
	m_camera = NewGO<CGameCamera>(0);
	m_spectator = NewGO<Spectator>(0);
	m_spectator->SetModelFilePath(FilePath::nathanMale);
	m_spectator->SetAnimFilePath(AnimPath::nathanMaleAnim);
	m_spectator->SetShadowCasterFlag(true);
	m_level.Init("Assets/level/cullingTest.tkl", [&](SLevelObjectData& objData) {
		if (objData.EqualObjectName(L"nathanMale")) {
			//オブジェクトの情報を追加。
			m_spectator->AddObjectData(objData.position, objData.rotation, objData.scale);

			return true;
		}

		return false;
		});

	GraphicsEngine()->GetShadowMap()->SetLightDirection({ 0.1f,-1.0f,0.1f });

	m_light = NewGO <prefab::CDirectionLight>(0);
	m_light->SetDirection({ 0.1f,-1.0f,0.1f });
	CFade::GetInstance()->StartFadeIn();

	return true;
}

void CullingTestScene::OnDestroy()
{
	DeleteGO(m_camera);
	DeleteGO(m_spectator);
}

void CullingTestScene::Update()
{
}
