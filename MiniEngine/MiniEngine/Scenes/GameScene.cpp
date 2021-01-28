#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera/GameCamera.h"
#include "Car/Car.h"
#include "Spectator/Spectator.h"
#include "Spectator/SpectatorNames.h"
#include "GameLight/SceneLight.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::Start()
{
	bool ret = false;

	switch (m_initState)
	{
	case GameScene::enInit_Athor:
		m_camera = NewGO<CGameCamera>(0);		//カメラ。

		m_light = NewGO<SceneLight>(0);			//照明。

		m_initState = enInit_Course;
		break;
	case GameScene::enInit_Course:
		m_courseLevel.Init("Assets/level/RaceLevel.tkl", [&](SLevelObjectData& objData) {
			if (wcscmp(objData.name, L"Sup") == 0) {
				//m_player = NewGO<Player>(0);
				m_car = NewGO<Car>(0);
				m_car->SetPosition(objData.position);
				m_car->SetRotation(objData.rotation);

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
		m_initState = enInit_Spectator;

		break;
	case GameScene::enInit_Spectator:
		//観客をロード。
		for (int i = 0; i < enSpectatorNum; i++) {
			m_spectator[i] = NewGO<Spectator>(0);
		}

		m_spectatorLevel.Init("Assets/level/spectatorLevel.tkl", [&](SLevelObjectData& objData) {
			if (objData.EqualObjectName(L"nathanMale")) {
				//オブジェクトの情報を追加。
				m_spectator[enNathanMale]->AddObjectData(
					objData.position,
					objData.rotation,
					objData.scale
				);
				m_spectator[enNathanMale]->SetShadowCasterFlag(objData.isShadowCaster);
				m_spectator[enNathanMale]->SetShadowReceiverFlag(objData.isShadowReceiver);

				return true;
			}
			if (objData.EqualObjectName(L"ShirtMale")) {
				//オブジェクトの情報を追加。
				m_spectator[enShirtMale]->AddObjectData(
					objData.position,
					objData.rotation,
					objData.scale
				);
				m_spectator[enShirtMale]->SetShadowCasterFlag(objData.isShadowCaster);
				m_spectator[enShirtMale]->SetShadowReceiverFlag(objData.isShadowReceiver);

				return true;
			}
			if (objData.EqualObjectName(L"sophiaWoman")) {
				//オブジェクトの情報を追加。
				m_spectator[enSophiaWoman]->AddObjectData(
					objData.position,
					objData.rotation,
					objData.scale
				);
				m_spectator[enSophiaWoman]->SetShadowCasterFlag(objData.isShadowCaster);
				m_spectator[enSophiaWoman]->SetShadowReceiverFlag(objData.isShadowReceiver);

				return true;
			}
			if (objData.EqualObjectName(L"whiteWoman")) {
				//オブジェクトの情報を追加。
				m_spectator[enClaudiaWoman]->AddObjectData(
					objData.position,
					objData.rotation,
					objData.scale
				);
				m_spectator[enClaudiaWoman]->SetShadowCasterFlag(objData.isShadowCaster);
				m_spectator[enClaudiaWoman]->SetShadowReceiverFlag(objData.isShadowReceiver);

				return true;
			}
			//使わなくなったもの。存在している可能性考慮して残しとくが、そのうち消す。
			if (objData.EqualObjectName(L"womanSuit")) {
				return true;
			}

			return false;
			});

		//モデルとアニメーションの指定。
		m_spectator[enNathanMale]->SetModelFilePath(FilePath::nathanMale);
		m_spectator[enNathanMale]->SetAnimFilePath(AnimPath::nathanMaleAnim);

		m_spectator[enShirtMale]->SetModelFilePath(FilePath::shirtMale);
		m_spectator[enShirtMale]->SetAnimFilePath(AnimPath::shirtMaleAnim);

		m_spectator[enSophiaWoman]->SetModelFilePath(FilePath::sophiaWoman);
		m_spectator[enSophiaWoman]->SetAnimFilePath(AnimPath::sophiaWomanAnim);

		m_spectator[enClaudiaWoman]->SetModelFilePath(FilePath::claudiaWoman);
		m_spectator[enClaudiaWoman]->SetAnimFilePath(AnimPath::claudiaWomanAnim);

		ret = true;
		break;
	}

	return ret;
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

