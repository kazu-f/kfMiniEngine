#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera/GameCamera.h"
#include "Car/Car.h"
#include "Spectator/Spectator.h"
#include "GameLight/SceneLight.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::Start()
{

	m_camera = NewGO<CGameCamera>(0);

	m_spectator = NewGO<Spectator>(0);

	m_light = NewGO<SceneLight>(0);

	m_level.Init("Assets/level/RaceLevel.tkl", [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"Sup") == 0) {
			//m_player = NewGO<Player>(0);
			m_car = NewGO<Car>(0);
			m_car->SetPosition(objData.position);
			m_car->SetRotation(objData.rotation);

			return true;
		}
		if (objData.EqualObjectName(L"womanSuit")) {
			m_spectator->AddObjectData(
				objData.position, 
				objData.rotation, 
				objData.scale
			);
			m_spectator->SetShadowCasterFlag(objData.isShadowCaster);
			m_spectator->SetShadowReceiverFlag(objData.isShadowReceiver);

			return true;
		}

		return false;
		});
		if (m_car != nullptr)
		{
			m_car->SetCamera(m_camera);
		}
		else
		{
			DEBUG_LOG("ƒŒƒxƒ‹‚ÅŽÔ‚ªŒ©‚Â‚©‚ç‚È‚©‚Á‚½B")
		}

	return true;
}

void GameScene::Release()
{
}

void GameScene::PreUpdate()
{

}

void GameScene::Update()
{
	Quaternion qRot;
	if (g_pad[0]->IsPress(enButtonRight)) {
		qRot.SetRotationDegY(1.0f);
	}
	else if (g_pad[0]->IsPress(enButtonLeft)) {
		qRot.SetRotationDegY(-1.0f);
	}

	////ŠÂ‹«Œõ‚Ì•Ï‰»‚ð‚³‚¹‚éB
	//m_lightPow += g_pad[0]->GetLStickYF() * 0.02f;
	//m_lightPow = min(1.0f, max(0.0f, m_lightPow));
	//g_graphicsEngine->GetLightManager()->SetAmbientLight({ m_lightPow,m_lightPow,m_lightPow });


}

