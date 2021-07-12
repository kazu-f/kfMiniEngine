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
#define IS_SPECTATOR ON			//�ϋq���o�����ǂ����B
#define DEBUG_DELETE ON				//�폜�������Ăяo����悤�ɂ��邩�B

namespace {
#if 0
	const char* COURSE_PATH = "Assets/level/CourseLevel.tkl";			//�v���C���[�̂�
	const char* SPECTATOR_PATH = "Assets/level/spectatorLevel2.tkl";
#else
	const char* COURSE_PATH = "Assets/level/CourseLevel2.tkl";			//CPU����B
	const char* SPECTATOR_PATH = "Assets/level/spectatorLevel2.tkl";
#endif
	const char* GUARDRAIL_ROAD = "Assets/level/RoadGuardrailLevel.tkl";
	const char* GUARDRAIL_TURNROAD = "Assets/level/RoadTurnGuardrailLevel.tkl";
	const char* CHECKPOINT_PATH = "Assets/level/CheckPointLevel.tkl";

	const int PHYSICS_DEBUGMODE = 0;

	const float SKY_SIZE = 3500.0f;		//��̃T�C�Y�B
	const float SKY_EMISSION_POW = 0.06f;	//��̎��Ȕ����B

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
		//�J�����⃉�C�g�Ȃǂ̏������B
		InitOther();
		m_initState = enInit_Course;
		break;
	case GameScene::enInit_Course:
		//�R�[�X��ǂݍ��ށB
		InitCourse();

		m_initState = enInit_Tree;
		break;

	case GameScene::enInit_Tree:
		InitTree();

		m_initState = enInit_Spectator;
		break;
	case GameScene::enInit_Spectator:
#if IS_SPECTATOR
		//�ϋq��z�u����B
		InitSpectator();
#endif
		m_initState = enInit_RaceController;
		break;
	case EnInitStep::enInit_RaceController:
		//���[�X����̃N���X���������B
		InitRaceController();

		m_initState = enInit_CheckPoint;
		break;
	case EnInitStep::enInit_CheckPoint:
		//�`�F�b�N�|�C���g��ǂݍ��ށB
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
	DeleteGO(m_camera);		//�J�����B
	DeleteGO(m_playerCar);		//�ԁB
	DeleteGO(m_cpuCar);		//�ԁB
	DeleteGO(m_ground);		//�w�i���f���B
	DeleteGO(m_trees);
	for (int i = 0; i < enSpectatorNum; i++)
	{
		//�ϋq�B
		DeleteGO(m_spectator[i]);
	}
	DeleteGO(m_guardrail);	//�K�[�h���[���B
	DeleteGO(m_light);		//���C�g�B
	DeleteGO(m_checkPointManager);	//�`�F�b�N�|�C���g�B
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
			//�������c�ɐ؂�ւ��B
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
	m_camera = NewGO<FollowCarCamera>(5);		//�J�����B

	m_light = NewGO<SceneLight>(0);			//�Ɩ��B

	m_sky = NewGO<prefab::CSky>(5, "Sky");			//��B
	m_sky->SetScale(SKY_SIZE);
	m_sky->SetEmissionColor({ SKY_EMISSION_POW, SKY_EMISSION_POW, SKY_EMISSION_POW });

	//BGM
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->InitStreaming(RACE_BGM);
	m_bgm->SetVolume(0.5f);
}

void GameScene::InitCourse()
{
	m_guardrail = NewGO<Guardrail>(0);		//�K�[�h���[���B

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
			//�K�[�h���[����ǂݍ���ł����B
			m_guardrail->LoadGuardrail(GUARDRAIL_ROAD, objData.position, objData.rotation);
		}
		if (objData.EqualObjectName(L"Road/SM_TurnRoad"))
		{
			//�K�[�h���[����ǂݍ���ł����B
			m_guardrail->LoadGuardrail(GUARDRAIL_TURNROAD, objData.position, objData.rotation);
		}
		if (objData.EqualObjectName(L"Road/Ground"))
		{
			//�n�ʂ����B
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
			"���x���ŎԂ�������Ȃ������B"
			":tkl�t�@�C���̃f�[�^���m�F�B"
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
			//�I�u�W�F�N�g�̏���ǉ��B
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
	//�ϋq�����[�h�B
	for (int i = 0; i < enSpectatorNum; i++) {
		m_spectator[i] = NewGO<Spectator>(0);
	}

	m_spectatorLevel.Init(SPECTATOR_PATH, [&](SLevelObjectData& objData) {
		if (objData.EqualObjectName(L"nathanMale")) {
			//�I�u�W�F�N�g�̏���ǉ��B
			AddSpectatorData(objData, enNathanMale);

			return true;
		}
		if (objData.EqualObjectName(L"ShirtMale")) {
			//�I�u�W�F�N�g�̏���ǉ��B
			AddSpectatorData(objData, enShirtMale);

			return true;
		}
		if (objData.EqualObjectName(L"sophiaWoman")) {
			//�I�u�W�F�N�g�̏���ǉ��B
			AddSpectatorData(objData, enSophiaWoman);

			return true;
		}
		if (objData.EqualObjectName(L"whiteWoman")) {
			//�I�u�W�F�N�g�̏���ǉ��B
			AddSpectatorData(objData, enClaudiaWoman);

			return true;
		}
		//�g��Ȃ��Ȃ������́B���݂��Ă���\���l�����Ďc���Ƃ����A���̂��������B
		if (objData.EqualObjectName(L"womanSuit")) {
			return true;
		}

		return false;
		});

	//���f���ƃA�j���[�V�����̎w��B
	for (int i = 0; i < EnSpectator::enSpectatorNum; i++) {
		m_spectator[i]->SetModelFilePath(FilePath::SPECTATOR_MODEL_PATH[i]);
		m_spectator[i]->SetAnimFilePath(AnimPath::SPECTATOR_ANIM_PATH[i]);
	}

#endif
}

//���[�X�̐�����s���N���X���������B
void GameScene::InitRaceController()
{
	//���[�X�̐���N���X�B
	m_raceController = NewGO<RaceController>(3);
	//�v���C���[��ݒ肷��B
	m_raceController->SetPlayer(m_playerCar);
	//CPU��ݒ�B
	if (m_cpuCar != nullptr) {
		m_raceController->RegistCPU(m_cpuCar);
	}
}

void GameScene::InitCheckPoint()
{
	//�`�F�b�N�|�C���g��ǂݍ��ށB
	m_checkPointManager = NewGO<CheckPointManager>(2);
	//�`�F�b�N�|�C���g�̃��x���B
	CLevel checkPointLevel;
	checkPointLevel.Init(CHECKPOINT_PATH, [&](SLevelObjectData& objData)
		{
			//�`�F�b�N�|�C���g�𒲂ׂĂ����B
			if (objData.ForwardMatchName(L"CheckPoint_"))
			{
				//���O����`�F�b�N�|�C���g�ԍ��𒲂ׂ�B
				const wchar_t* objName = objData.name;
				//�����̎�O�̃A�h���X���擾�B
				const wchar_t* index = wcsstr(objName, L"_");
				if (index == nullptr) {
					return true;
				}
				index++;		//�`�F�b�N�|�C���g�ԍ��܂ňʒu���ړ��B
				if (wcscmp(index, L"Start") == 0) {
					//�X�^�[�g�n�_�̃`�F�b�N�|�C���g���쐬�B
					m_checkPointManager->MakeCheckPoint(
						objData.position,
						objData.rotation,
						0
					);
				}
				else if (wcscmp(index, L"Goal") == 0) {
					//�S�[���p�̃`�F�b�N�|�C���g���쐬�B
					m_checkPointManager->MakeGoalPoint(
						objData.position,
						objData.rotation
					);
				}
				else {
					//�`�F�b�N�|�C���g���쐬�B
					m_checkPointManager->MakeCheckPoint(
						objData.position,
						objData.rotation,
						_wtoi(index)
					);
				}

			}

			//���f���\���͕s�v�B
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
	//�I�u�W�F�N�g�̏���ǉ��B
	m_spectator[type]->AddObjectData(
		objData.position,
		objData.rotation,
		objData.scale
	);
	m_spectator[type]->SetShadowCasterFlag(objData.isShadowCaster);
	m_spectator[type]->SetShadowReceiverFlag(objData.isShadowReceiver);

}

