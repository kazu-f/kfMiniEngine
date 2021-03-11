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
#define IS_SPECTATOR OFF			//�ϋq���o�����ǂ����B
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
		m_camera = NewGO<FollowCarCamera>(5);		//�J�����B

		m_light = NewGO<SceneLight>(0);			//�Ɩ��B

		m_initState = enInit_Course;
		break;
	case GameScene::enInit_Course:
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
		m_initState = enInit_Spectator;

		break;
	case GameScene::enInit_Spectator:
#if IS_SPECTATOR
		//�ϋq�����[�h�B
		for (int i = 0; i < enSpectatorNum; i++) {
			m_spectator[i] = NewGO<Spectator>(0);
		}

		m_spectatorLevel.Init(SPECTATOR_PATH, [&](SLevelObjectData& objData) {
			if (objData.EqualObjectName(L"nathanMale")) {
				//�I�u�W�F�N�g�̏���ǉ��B
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
				//�I�u�W�F�N�g�̏���ǉ��B
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
				//�I�u�W�F�N�g�̏���ǉ��B
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
				//�I�u�W�F�N�g�̏���ǉ��B
				m_spectator[enClaudiaWoman]->AddObjectData(
					objData.position,
					objData.rotation,
					objData.scale
				);
				m_spectator[enClaudiaWoman]->SetShadowCasterFlag(objData.isShadowCaster);
				m_spectator[enClaudiaWoman]->SetShadowReceiverFlag(objData.isShadowReceiver);

				return true;
			}
			//�g��Ȃ��Ȃ������́B���݂��Ă���\���l�����Ďc���Ƃ����A���̂��������B
			if (objData.EqualObjectName(L"womanSuit")) {
				return true;
			}

			return false;
			});

		//���f���ƃA�j���[�V�����̎w��B
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
		//�`�F�b�N�|�C���g��ǂݍ��ށB
		m_checkPointManager = NewGO<CheckPointManager>(5);
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

	////�����̕ω���������B
	//m_lightPow += g_pad[0]->GetLStickYF() * 0.02f;
	//m_lightPow = min(1.0f, max(0.0f, m_lightPow));
	//g_graphicsEngine->GetLightManager()->SetAmbientLight({ m_lightPow,m_lightPow,m_lightPow });


}

