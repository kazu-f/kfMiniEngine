#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera/FollowCarCamera.h"
#include "Car/Car.h"
#include "Spectator/Spectator.h"
#include "GameLight/SceneLight.h"
#include "Guardrail/Guardrail.h"
#include "CheckPoint/CheckPointManager.h"
#include "Ground/BackGround.h"
#include "Race/RaceController.h"

#define ON 1
#define OFF 0
#define IS_SPECTATOR ON			//�ϋq���o�����ǂ����B
#define DEBUG_DELETE ON				//�폜�������Ăяo����悤�ɂ��邩�B

namespace {
#if 0								//�����g�����Ƃ͂Ȃ������H
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
		//�J�����⃉�C�g�Ȃǂ̏������B
		InitOther();
		m_initState = enInit_Course;
		break;
	case GameScene::enInit_Course:
		//�R�[�X��ǂݍ��ށB
		InitCourse();

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

		m_initState = enInit_End;
		ret = true;
		break;
	}

	return ret;
}

void GameScene::Release()
{
	DeleteGO(m_camera);		//�J�����B
	DeleteGO(m_car);		//�ԁB
	DeleteGO(m_ground);		//�w�i���f���B
	for (int i = 0; i < enSpectatorNum; i++)
	{
		//�ϋq�B
		DeleteGO(m_spectator[i]);
	}
	DeleteGO(m_guardrail);	//�K�[�h���[���B
	DeleteGO(m_light);		//���C�g�B
	DeleteGO(m_checkPointManager);	//�`�F�b�N�|�C���g�B
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

#if DEBUG_DELETE

	if (g_pad[0]->IsTrigger(enButtonStart))
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
}

void GameScene::InitCourse()
{
	m_guardrail = NewGO<Guardrail>(0);		//�K�[�h���[���B

	m_courseLevel.Init(COURSE_PATH, [&](SLevelObjectData& objData) {
		if (wcscmp(objData.name, L"Sup") == 0) {
			m_car = NewGO<Car>(0);
			m_car->SetPosition(objData.position);
			m_car->SetRotation(objData.rotation);

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
	if (m_car != nullptr)
	{
		m_camera->SetTargetCar(m_car);
	}
	else
	{
		ENGINE_LOG(
			"���x���ŎԂ�������Ȃ������B"
			":tkl�t�@�C���̃f�[�^���m�F�B"
		)
	}
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
	m_raceController->SetPlayer(m_car->GetCheckedController());
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
	m_checkPointManager->RegistCheckedController(m_car->GetCheckedController());
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

