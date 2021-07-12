#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera/FollowCarCamera.h"
#include "Car/Car.h"
#include "Tree/TreeInstancing.h"
#include "Spectator/Spectator.h"
#include "GameLight/SceneLight.h"
#include "Guardrail/Guardrail.h"
#include "CheckPoint/CheckPointManager.h"
#include "Ground/BackGround.h"
#include "Race/RaceController.h"
#include "Title\TitleScene.h"

#define ON 1
#define OFF 0
#define IS_SPECTATOR ON			//観客を出すかどうか。
#define DEBUG_DELETE ON				//削除処理を呼び出せるようにするか。

namespace {
#if 0
	const char* COURSE_PATH = "Assets/level/CourseLevel.tkl";			//プレイヤーのみ
	const char* SPECTATOR_PATH = "Assets/level/spectatorLevel2.tkl";
#else
	const char* COURSE_PATH = "Assets/level/CourseLevel2.tkl";			//CPUあり。
	const char* SPECTATOR_PATH = "Assets/level/spectatorLevel2.tkl";
#endif
	const char* GUARDRAIL_ROAD = "Assets/level/RoadGuardrailLevel.tkl";
	const char* GUARDRAIL_TURNROAD = "Assets/level/RoadTurnGuardrailLevel.tkl";
	const char* CHECKPOINT_PATH = "Assets/level/CheckPointLevel.tkl";

	const int PHYSICS_DEBUGMODE = 0;

	const float SKY_SIZE = 3500.0f;		//空のサイズ。
	const float SKY_EMISSION_POW = 0.06f;	//空の自己発光。

	const wchar_t* RACE_BGM = L"Assets/sound/Race/Race.wav";
}


GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	NewGO<TitleScene>(0);
}

bool GameScene::Start()
{
	bool ret = false;

	PhysicsWorld().SetDebugMode(PHYSICS_DEBUGMODE);

	switch (m_initState)
	{
	case GameScene::enInit_Other:
		//カメラやライトなどの初期化。
		InitOther();
		m_initState = enInit_Course;
		break;
	case GameScene::enInit_Course:
		//コースを読み込む。
		InitCourse();

		m_initState = enInit_Tree;
		break;

	case GameScene::enInit_Tree:
		InitTree();

		m_initState = enInit_Spectator;
		break;
	case GameScene::enInit_Spectator:
#if IS_SPECTATOR
		//観客を配置する。
		InitSpectator();
#endif
		m_initState = enInit_RaceController;
		break;
	case EnInitStep::enInit_RaceController:
		//レース制御のクラスを初期化。
		InitRaceController();

		m_initState = enInit_CheckPoint;
		break;
	case EnInitStep::enInit_CheckPoint:
		//チェックポイントを読み込む。
		InitCheckPoint();
		
		GraphicsEngine()->GetPostEffect()->GetTonemap().Reset();

		m_bgm->Play(true);
		m_initState = enInit_End;
		ret = true;
		break;
	}
	if (m_raceController != nullptr)
	{
		m_raceController->SetInRaceScene(ret);
	}

	return ret;
}

void GameScene::Release()
{
	DeleteGO(m_camera);		//カメラ。
	DeleteGO(m_playerCar);		//車。
	DeleteGO(m_cpuCar);		//車。
	DeleteGO(m_ground);		//背景モデル。
	DeleteGO(m_trees);
	for (int i = 0; i < enSpectatorNum; i++)
	{
		//観客。
		DeleteGO(m_spectator[i]);
	}
	DeleteGO(m_guardrail);	//ガードレール。
	DeleteGO(m_light);		//ライト。
	DeleteGO(m_checkPointManager);	//チェックポイント。
	DeleteGO(m_raceController);
	DeleteGO(m_sky);
	DeleteGO(m_bgm);
}

void GameScene::PreUpdate()
{

}

void GameScene::Update()
{
	switch (m_raceState)
	{
	case GameScene::enState_InRace:
		if (m_playerCar->IsGoal())
		{
			//自動操縦に切り替え。
			m_playerCar->SetCarDriver(Car::EnDriverType::enTypeAI);
			m_raceState = enState_EndRace;
		}
		break;
	case GameScene::enState_EndRace:
		if (Pad(0).IsTrigger(enButtonA) && m_raceController->IsEndRace())
		{
			DeleteGO(this);
		}
		break;
	default:
		break;
	}

#if DEBUG_DELETE

	if (Pad(0).IsTrigger(enButtonStart))
	{
		DeleteGO(this);
	}

#endif // DEBUG_DELETE
}

void GameScene::OnDestroy()
{
	Release();
}

void GameScene::InitOther()
{
	m_camera = NewGO<FollowCarCamera>(5);		//カメラ。

	m_light = NewGO<SceneLight>(0);			//照明。

	m_sky = NewGO<prefab::CSky>(5, "Sky");			//空。
	m_sky->SetScale(SKY_SIZE);
	m_sky->SetEmissionColor({ SKY_EMISSION_POW, SKY_EMISSION_POW, SKY_EMISSION_POW });

	//BGM
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->InitStreaming(RACE_BGM);
	m_bgm->SetVolume(0.5f);
}

void GameScene::InitCourse()
{
	m_guardrail = NewGO<Guardrail>(0);		//ガードレール。

	m_courseLevel.Init(COURSE_PATH, [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"PlayerCar") == 0) {
			m_playerCar = NewGO<Car>(0);
			m_playerCar->SetPosition(objData.position);
			m_playerCar->SetRotation(objData.rotation);
			//m_playerCar->SetCarDriver(Car::EnDriverType::enTypeAI);
			m_playerCar->SetCarDriver(Car::EnDriverType::enTypePlayer);
			m_playerCar->SetCarColor(CAR::enCar_Red);
			m_playerCar->SetIsPlayer(true);

			return true;
		}
		if (wcscmp(objData.name, L"CPUCar") == 0) {
			m_cpuCar = NewGO<Car>(0);
			m_cpuCar->SetPosition(objData.position);
			m_cpuCar->SetRotation(objData.rotation);
			m_cpuCar->SetCarDriver(Car::EnDriverType::enTypeAI);
			//m_cpuCar->SetCarDriver(Car::EnDriverType::enTypePlayer);
			m_cpuCar->SetCarColor(CAR::enCar_Blue);

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
	if (m_playerCar != nullptr)
	{
		m_camera->SetTargetCar(m_playerCar);
	}
	else
	{
		ENGINE_LOG(
			"レベルで車が見つからなかった。"
			":tklファイルのデータを確認。"
		);
	}
}

void GameScene::InitTree()
{
	m_trees = NewGO<TreeInstancing>(0);
	m_trees->SetTreeFilePath(TREE::Alaska1);
	
	CLevel treeLevelData;
	treeLevelData.Init(TREE::LEVEL_FILEPATH, [&](SLevelObjectData& objData) {
		if (objData.EqualObjectName(L"Alaska1")) {
			//オブジェクトの情報を追加。
			m_trees->AddTreeObjData(
				objData.position,
				objData.rotation,
				objData.scale
			);

			return true;
		}

		return false;
		}
	);

}

void GameScene::InitSpectator()
{
#if IS_SPECTATOR
	//観客をロード。
	for (int i = 0; i < enSpectatorNum; i++) {
		m_spectator[i] = NewGO<Spectator>(0);
	}

	m_spectatorLevel.Init(SPECTATOR_PATH, [&](SLevelObjectData& objData) {
		if (objData.EqualObjectName(L"nathanMale")) {
			//オブジェクトの情報を追加。
			AddSpectatorData(objData, enNathanMale);

			return true;
		}
		if (objData.EqualObjectName(L"ShirtMale")) {
			//オブジェクトの情報を追加。
			AddSpectatorData(objData, enShirtMale);

			return true;
		}
		if (objData.EqualObjectName(L"sophiaWoman")) {
			//オブジェクトの情報を追加。
			AddSpectatorData(objData, enSophiaWoman);

			return true;
		}
		if (objData.EqualObjectName(L"whiteWoman")) {
			//オブジェクトの情報を追加。
			AddSpectatorData(objData, enClaudiaWoman);

			return true;
		}
		//使わなくなったもの。存在している可能性考慮して残しとくが、そのうち消す。
		if (objData.EqualObjectName(L"womanSuit")) {
			return true;
		}

		return false;
		});

	//モデルとアニメーションの指定。
	for (int i = 0; i < EnSpectator::enSpectatorNum; i++) {
		m_spectator[i]->SetModelFilePath(FilePath::SPECTATOR_MODEL_PATH[i]);
		m_spectator[i]->SetAnimFilePath(AnimPath::SPECTATOR_ANIM_PATH[i]);
	}

#endif
}

//レースの制御を行うクラスを初期化。
void GameScene::InitRaceController()
{
	//レースの制御クラス。
	m_raceController = NewGO<RaceController>(3);
	//プレイヤーを設定する。
	m_raceController->SetPlayer(m_playerCar);
	//CPUを設定。
	if (m_cpuCar != nullptr) {
		m_raceController->RegistCPU(m_cpuCar);
	}
}

void GameScene::InitCheckPoint()
{
	//チェックポイントを読み込む。
	m_checkPointManager = NewGO<CheckPointManager>(2);
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
	m_checkPointManager->RegistCheckedController(m_playerCar->GetCheckedController());
	if (m_cpuCar != nullptr) {
		m_checkPointManager->RegistCheckedController(m_cpuCar->GetCheckedController());
	}
	m_checkPointManager->EnableInit();
}

void GameScene::AddSpectatorData(SLevelObjectData& objData, int type)
{
	//オブジェクトの情報を追加。
	m_spectator[type]->AddObjectData(
		objData.position,
		objData.rotation,
		objData.scale
	);
	m_spectator[type]->SetShadowCasterFlag(objData.isShadowCaster);
	m_spectator[type]->SetShadowReceiverFlag(objData.isShadowReceiver);

}

