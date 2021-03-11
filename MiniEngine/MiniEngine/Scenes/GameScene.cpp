#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera/FollowCarCamera.h"
#include "Car/Car.h"
#include "Spectator/Spectator.h"
#include "Spectator/SpectatorNames.h"
#include "GameLight/SceneLight.h"
#include "Guardrail/Guardrail.h"
#include "CheckPoint/CheckPointManager.h"
#include "Ground/BackGround.h"

#define ON 1
#define OFF 0
#define IS_SPECTATOR OFF			//観客を出すかどうか。
namespace {
#if 0
	const char* COURSE_PATH = "Assets/level/CourseLevel.tkl";
	const char* SPECTATOR_PATH = "Assets/level/spectatorLevel.tkl";
#else
	const char* COURSE_PATH = "Assets/level/CourseLevel2.tkl";
	const char* SPECTATOR_PATH = "Assets/level/spectatorLevel2.tkl";
#endif
	const char* GUARDRAIL_ROAD = "Assets/level/RoadGuardrailLevel.tkl";
	const char* GUARDRAIL_TURNROAD = "Assets/level/RoadTurnGuardrailLevel.tkl";
	const char* CHECKPOINT_PATH = "Assets/level/CheckPointLevel.tkl";
}


GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::Start()
{
	bool ret = false;

	//PhysicsWorld().SetDebugMode(1);

	switch (m_initState)
	{
	case GameScene::enInit_Other:
		m_camera = NewGO<FollowCarCamera>(5);		//カメラ。

		m_light = NewGO<SceneLight>(0);			//照明。

		m_initState = enInit_Course;
		break;
	case GameScene::enInit_Course:
		m_guardrail = NewGO<Guardrail>(0);		//ガードレール。

		m_courseLevel.Init(COURSE_PATH, [&](SLevelObjectData& objData) {
			if (wcscmp(objData.name, L"Sup") == 0) {
				m_car = NewGO<Car>(0);
				m_car->SetPosition(objData.position);
				m_car->SetRotation(objData.rotation);

				return true;
			}

			if (objData.EqualObjectName(L"Road/SM_Road"))
			{
				//ガードレールを読み込んでいく。
				m_guardrail->LoadGuardrail(GUARDRAIL_ROAD, objData.position, objData.rotation);
			}
			if (objData.EqualObjectName(L"Road/SM_TurnRoad"))
			{
				//ガードレールを読み込んでいく。
				m_guardrail->LoadGuardrail(GUARDRAIL_TURNROAD, objData.position, objData.rotation);
			}
			if (objData.EqualObjectName(L"Road/Ground"))
			{
				//地面を作る。
				m_ground = NewGO<BackGround>(0);
				m_ground->InitData(
					objData.position,
					objData.rotation,
					objData.scale
				);

				return true;
			}


			return false;
			});
		if (m_car != nullptr)
		{
			m_camera->SetTargetCar(m_car);
		}
		else
		{
			ENGINE_LOG(
				"レベルで車が見つからなかった。"
				":tklファイルのデータを確認。"
			)
		}
		m_initState = enInit_Spectator;

		break;
	case GameScene::enInit_Spectator:
#if IS_SPECTATOR
		//観客をロード。
		for (int i = 0; i < enSpectatorNum; i++) {
			m_spectator[i] = NewGO<Spectator>(0);
		}

		m_spectatorLevel.Init(SPECTATOR_PATH, [&](SLevelObjectData& objData) {
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

		m_initState = enInit_CheckPoint;
		break;
#endif
	case EnInitStep::enInit_CheckPoint:
		//チェックポイントを読み込む。
		m_checkPointManager = NewGO<CheckPointManager>(5);
		//チェックポイントのレベル。
		CLevel checkPointLevel;
		checkPointLevel.Init(CHECKPOINT_PATH, [&](SLevelObjectData& objData) 
			{
				//チェックポイントを調べていく。
				if (objData.ForwardMatchName(L"CheckPoint_"))
				{
					//名前からチェックポイント番号を調べる。
					const wchar_t* objName = objData.name;
					//数字の手前のアドレスを取得。
					const wchar_t* index = wcsstr(objName, L"_");
					if (index == nullptr) {
						return true;
					}
					index++;		//チェックポイント番号まで位置を移動。
					if (wcscmp(index, L"Start") == 0) {
						//スタート地点のチェックポイントを作成。
						m_checkPointManager->MakeCheckPoint(
							objData.position,
							objData.rotation,
							0
						);
					}
					else if (wcscmp(index, L"Goal") == 0) {
						//ゴール用のチェックポイントを作成。
						m_checkPointManager->MakeGoalPoint(
							objData.position,
							objData.rotation
						);
					}
					else {
						//チェックポイントを作成。
						m_checkPointManager->MakeCheckPoint(
							objData.position,
							objData.rotation,
							_wtoi(index)
						);
					}

				}

				//モデル表示は不要。
				return true;
			}
		);
		m_checkPointManager->RegistCheckedController(m_car->GetCheckedController());
		m_checkPointManager->EnableInit();

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

