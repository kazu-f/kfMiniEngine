#include "stdafx.h"
#include "Car.h"
#include "CarDriver/ICarDriver.h"
#include "CarDriver/PlayerCarDriver.h"
#include "CarDriver/AICarDriver.h"
#include "CheckPoint/CheckedController.h"
#include "GameCamera/GameCamera.h"
#include "SpeedoMeter.h"
#include "CarSoundController.h"

namespace {

	//�Ԃ̃��f��
	const char* CAR_MODEL[CAR::enCar_Num] =
	{
		"Assets/modelData/Car/Car.tkm",
		"Assets/modelData/Car/Car_B.tkm",
	};
}


Car::Car()
{
	m_checkedCon = std::make_unique<CheckedController>();
	m_carMoveCon = std::make_unique<CarMoveController>();
	m_soundCon = std::make_unique<CarSoundController>();
}

Car::~Car()
{
}

bool Car::Start()
{
	bool ret = false;
	//3D���f�����쐬�B
	ModelInitData initData;
	switch (m_startStep)
	{
	case en_initCar:
		initData.m_tkmFilePath = CAR_MODEL[m_carColor];
		m_model = NewGO<prefab::ModelRender>(0);
		m_model->Init(initData);
		//���f���̐ݒ�B
		m_model->SetPosition(m_position);
		m_model->SetRotation(m_rotation);
		m_model->SetShadowCasterFlag(true);
		m_model->SetShadowReceiverFlag(true);


		//�`�F�b�N�|�C���g�R���g���[���[�̏������B
		m_checkedCon->Init(
			m_carMoveCon->GetCharaCon().GetBody(),
			m_position,
			m_rotation
		);

		//�ړ�����N���X�̏������B
		m_carMoveCon->Init(m_position, m_rotation);
		//���݂̃X�e�[�g���������B
		m_carMoveCon->ChangeState(CarMoveController::EnCarState::enStateIdle);

		//�T�E���h�̏������B
		m_soundCon->Init();

		if (m_isPlayer)
		{
			m_speedoMeter = std::make_unique<SpeedoMeter>();
			m_speedoMeter->Init();
		}

		m_startStep = en_waitCountDown;
		break;
	case en_waitCountDown:
		if (m_isRaceCountDown) m_startStep = en_waitRaceStart;
		break;
	case en_waitRaceStart:
		m_soundCon->SetCarVolume(CFade::GetInstance()->GetFadeRate());
		m_soundCon->UpdateCarSound(this);
		ret = m_isRaceStart;
		break;
	default:
		break;
	}

	return ret;
}

void Car::Update()
{
	if (m_carDriver.get() == nullptr) return;

	//�h���C�o�̍X�V�B
	m_carDriver->Update();
	//�ړ����s���B
	if (!m_carMoveCon->MoveCar()) return;

	//���W�Ɖ�]���X�V�B
	m_position = m_carMoveCon->GetPosition();
	m_rotation = m_carMoveCon->GetRotation();

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	m_checkedCon->Update(m_position, m_rotation);
	//�T�E���h�̍X�V�B
	m_soundCon->SetCarVolume(CFade::GetInstance()->GetFadeRate());
	m_soundCon->UpdateCarSound(this);

	//���x���[�^�[�B
	if (m_isPlayer)
	{
		m_speedoMeter->UpdateSpeed(m_carMoveCon->GetSpeed());
	}

	//if (g_pad[0]->IsTrigger(enButtonX)) {
	//	DeleteGO(this);
	//}
}

void Car::OnDestroy()
{
	DeleteGO(m_model);
	m_soundCon->Release();
	if (m_isPlayer)
	{
		m_speedoMeter->Release();
	}
}

void Car::SetCarDriver(EnDriverType type)
{
	ENGINE_ASSERT(
		0 <= type || type < enDriverTypeNum,
		"�Ԃ̃h���C�o�[�̎�ނ𐳂����w��ł��Ă��܂���B\n"
		"Car.h��EnDriverType���m�F���Ă��������B"
	);

	if (m_carDriver.get() != nullptr) {
		ENGINE_LOG("���ɎԂ̃h���C�o���ݒ肳��Ă��܂��B");
		m_carDriver.reset();
	}

	switch (type)
	{
	case EnDriverType::enTypePlayer:
		m_carDriver = std::make_unique<PlayerCarDriver>();
		break;

	case EnDriverType::enTypeAI:
		m_carDriver = std::make_unique<AICarDriver>(this);
		break;
	default:
		break;
	}

	m_carMoveCon->SetCarDriver(m_carDriver.get());
}

bool Car::IsGoal()
{
	return m_checkedCon->IsGoal();
}


