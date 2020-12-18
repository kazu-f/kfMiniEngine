#include "stdafx.h"
#include "GameScene.h"
#include "prefab/light/DirectionLight.h"
#include "GameCamera.h"
#include "Player.h"
#include "Car/Car.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::Start()
{
	//ライトを用意する。
	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);

	Vector4 color = Vector4::Gray * 1.0f;
	m_lightDir = { 0.0f,0.0f,-1.0f };
	//color = { 1.0f,1.0f,0.0f,1.0f };
	light->SetColor(color);
	light->SetDirection(m_lightDir);

	m_lightArray.push_back(light);

	prefab::CDirectionLight* light2 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 0.0f,0.0f,1.0f };
	//color = { 0.0f,1.0f,1.0f,1.0f };
	light2->SetColor(color);
	light2->SetDirection(m_lightDir);

	m_lightArray.push_back(light2);

	prefab::CDirectionLight* light3 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 0.0f,-1.0f,0.0f };
	//color = { 0.0f,1.0f,1.0f,1.0f };
	light3->SetColor(color);
	light3->SetDirection(m_lightDir);

	m_lightArray.push_back(light3);

	m_camera = NewGO<CGameCamera>(0);

#if 0
	m_level.Init("Assets/level/testLevel.tkl", [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"unityChan") == 0) {
			m_player = NewGO<Player>(0);

			return true;
		}

		return false;
		});
#else
	m_level.Init("Assets/level/RaceLevel.tkl", [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"Sup") == 0) {
			//m_player = NewGO<Player>(0);
			m_car = NewGO<Car>(0);
			m_car->SetPosition(objData.position);

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
			DEBUG_LOG("レベルで車が見つからなかった。")
		}
#endif


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

	////環境光の変化をさせる。
	//m_lightPow += g_pad[0]->GetLStickYF() * 0.02f;
	//m_lightPow = min(1.0f, max(0.0f, m_lightPow));
	//g_graphicsEngine->GetLightManager()->SetAmbientLight({ m_lightPow,m_lightPow,m_lightPow });


}

void GameScene::PreRender(RenderContext& rc)
{

}
