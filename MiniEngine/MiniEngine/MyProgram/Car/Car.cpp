#include "stdafx.h"
#include "Car.h"
#include "State/ICarState.h"
#include "State/CarStateIdle.h"
#include "State/CarStateAccele.h"
#include "State/CarStateBrake.h"
#include "State/CarStateDrift.h"
#include "GameCamera/GameCamera.h"
#include "CheckPoint/CheckedController.h"
#include "CarDriver/PlayerCarDriver.h"
#include "CarDriver/AICarDriver.h"

namespace {

	//�Ԃ̃��f��
	const char* CAR_MODEL[CAR::enCar_Num] =
	{
		"Assets/modelData/Car/Car.tkm",
		"Assets/modelData/Car/Car_B.tkm",
	};

	const float GRAVITY = 980.0f;			//�d�͉����x�B
	const float HANDLE_WEIGHT = 0.3f;		//�n���h���̌����B
	const float CURVE_DEG = 15.0f;			//�J�[�u�̊p�x�B
	const float DRIFT_POWER = 1.4f;			//�h���t�g�̋����B

	const float DEADROT_SPEED = 0.1f;		//���x�����ȉ��Ȃ��]���Ȃ��B
}

const float Car::MOVE_COEFFICIENT = 60.0f;

Car::Car()
{
	//�X�e�[�g���������B
	m_stateIdle = std::make_unique<CarStateIdle>(this);
	m_stateAccele = std::make_unique<CarStateAccele>(this);
	m_stateBrake = std::make_unique<CarStateBrake>(this);
	m_stateDrift = std::make_unique<CarStateDrift>(this);
	m_checkedCon = std::make_unique<CheckedController>();
}

Car::~Car()
{
}

bool Car::Start()
{
	//3D���f�����쐬�B
	ModelInitData initData;
	initData.m_tkmFilePath = CAR_MODEL[m_carColor];
	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData);
	//���f���̐ݒ�B
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetShadowCasterFlag(true);
	m_model->SetShadowReceiverFlag(true);

	//�L�����R���̏������B
	m_charaCon.Init(
		50.0f,
		50.0f,
		m_position
	);

	//�`�F�b�N�|�C���g�R���g���[���[�̏������B
	m_checkedCon->Init(
		m_charaCon.GetBody(),
		m_position,
		m_rotation
	);

	//���݂̃X�e�[�g���������B
	ChangeState(m_stateIdle.get());

	return true;
}

void Car::Update()
{
	if (m_currentState == nullptr) return;

	if (m_carDriver.get() == nullptr) return;

	m_carDriver->Update();

	CharaConMove();

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	m_checkedCon->Update(m_position, m_rotation);

	//if (g_pad[0]->IsTrigger(enButtonX)) {
	//	DeleteGO(this);
	//}
}

void Car::OnDestroy()
{
	DeleteGO(m_model);
}

void Car::ChangeState(ICarState* state)
{
	//�ω��Ȃ��B
	if (m_currentState == state)
	{
		return;
	}

	if (m_currentState != nullptr)
	{
		//���݃X�e�[�g�̎��㏈���B
		m_currentState->Leave();
	}
	//�X�e�[�g��؂�ւ��B
	m_currentState = state;
	//���O�����B
	m_currentState->Enter();
}


void Car::SetCarDriver(EnDriverType type)
{
	ENGINE_ASSERT(
		0 <= type || type < enDriverTypeNum,
		"�Ԃ̃h���C�o�[�̎�ނ𐳂����w��ł��Ă��܂���B\n"
		"Car.h��EnDriverType���m�F���Ă��������B"
	);

	ENGINE_ASSERT(
		m_carDriver.get() == nullptr,
		"�Ԃ̃h���C�o�[�����ɐݒ肳��Ă��܂��B"
	)

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

}

void Car::AddDicceleration(const float diccele)
{
	float MinSpeed = 0.0f;
	if (m_currentState->IsDrift())
	{
		MinSpeed = 1000.0f;
	}
	m_speed = max(MinSpeed, m_speed - (diccele * MOVE_COEFFICIENT * GameTime().GetFrameDeltaTime()));
}

void Car::CalcDirection()
{
	//�N�I�[�^�j�I��������������߂�B
	Matrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	//������������B
	m_forward = { mRot.m[2][0],mRot.m[2][1] ,mRot.m[2][2] };
	m_right = { mRot.m[0][0],mRot.m[0][1] ,mRot.m[0][2] };
	m_up = { mRot.m[1][0],mRot.m[1][1] ,mRot.m[1][2] };
}

void Car::CharaConMove()
{
	//const float HandlePower = Pad(0).GetLStickXF();
	const float HandlePower = m_carDriver->GetDriverHandle();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	//�X�e�[�g���s�B
	m_currentState->Execute();

	//���������߂�B
	//��]���y��������B
	Quaternion dirRot;
	float rotSpeed = CURVE_DEG;
	if (m_currentState->IsDecelerate()) {
		//�������̓J�[�u���₷���B
		rotSpeed *= DRIFT_POWER;
	}
	dirRot.SetRotationDegY(Math::PI * rotSpeed * HandlePower * DeltaTime);

	if (m_speed > DEADROT_SPEED) {
		m_rotation.Multiply(dirRot);
	}
	CalcDirection();

	//�ړ����x�����߂�B
	m_moveSpeed.x = m_forward.x * m_speed;
	m_moveSpeed.z = m_forward.z * m_speed;

	//��]����������v�Z�B
	CalcDirection();

	m_moveSpeed.y -= GRAVITY;

	m_position = m_charaCon.Execute(m_moveSpeed, DeltaTime);
}
