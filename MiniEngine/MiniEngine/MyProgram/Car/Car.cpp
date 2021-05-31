#include "stdafx.h"
#include "Car.h"
#include "State/ICarState.h"
#include "State/CarStateIdle.h"
#include "State/CarStateAccele.h"
#include "State/CarStateBrake.h"
#include "GameCamera/GameCamera.h"
#include "CheckPoint/CheckedController.h"

#define ISRigidBody 0

namespace {
	const float GRAVITY = 980.0f;			//�d�͉����x�B
	const float HANDLE_WEIGHT = 0.3f;		//�n���h���̌����B
	const float CURVE_DEG = 20.0f;			//�J�[�u�̊p�x�B
	const float DRIFT_POWER = 1.4f;			//�h���t�g�̋����B

	const float DEADROT_SPEED = 0.1f;		//���x�����ȉ��Ȃ��]���Ȃ��B

	const Vector3 CAR_SIZE = { 150.0f,150.0f,300.0f };			//�Ԃ̃T�C�Y�B
	const float HEIGHT_DIF = 300.0f;							//�R���C�_�[�Ƃ̍����B
	const Vector3 CAR_LOCALINTERIA = { 0.0f,0.0f,0.0f };		//�Ԃ̉�]�̂��₷���B
	const Vector3 CAR_LINERFACTOR = { 1.0f,1.0f,1.0f };			//�ړ��\���B
	const Vector3 CAR_ANGULARFACTOR = { 1.0f,1.0f,1.0f };		//��]����ݒ�B
	const float CAR_MASS = 3.0f;								//�Ԃ̎��ʁB
	const float CAR_FRICTION = 0.0f;							//�Ԃ̖��C�́B

}

const float Car::MOVE_COEFFICIENT = 60.0f;

Car::Car()
{
	//�X�e�[�g���������B
	m_stateIdle = std::make_unique<CarStateIdle>(this);
	m_stateAccele = std::make_unique<CarStateAccele>(this);
	m_stateBrake = std::make_unique<CarStateBrake>(this);
	m_checkedCon = std::make_unique<CheckedController>();
}

Car::~Car()
{
}

bool Car::Start()
{
	//3D���f�����쐬�B
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/Car/Car.tkm";
	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData);
	//���f���̐ݒ�B
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetShadowCasterFlag(true);
	m_model->SetShadowReceiverFlag(true);

#if ISRigidBody
	//�Ԃ̃R���W�������쐬�B
	m_carCollider.Create(CAR_SIZE);

	Vector3 pos = m_position;
	pos.y += HEIGHT_DIF;
	//���̂��쐬����B
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_carCollider;
	rbInfo.pos = pos;
	rbInfo.rot = m_rotation;
	rbInfo.localInteria = CAR_LOCALINTERIA;
	rbInfo.mass = CAR_MASS;
	m_rigidBody.Create(rbInfo);
	//���C�͂̐ݒ�B
	m_rigidBody.SetFriction(CAR_FRICTION);
	//�ړ��\���B
	m_rigidBody.SetLinearFactor(CAR_LINERFACTOR);
	//��]����ݒ�B
	m_rigidBody.SetAngularFactor(CAR_ANGULARFACTOR);
	//�d�͂�ݒ�B
	m_rigidBody.SetGravity(GRAVITY * 100.0f);

	//�`�F�b�N�|�C���g�R���g���[���[�̏������B
	m_checkedCon->Init(
		&m_rigidBody,
		m_position,
		m_rotation
	);
#else

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
#endif

	//���݂̃X�e�[�g���������B
	ChangeState(m_stateIdle.get());

	return true;
}

void Car::Update()
{
	if (m_currentState == nullptr) return;

#if ISRigidBody
	RigidBodyMove();
#else
	CharaConMove();
#endif

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
	const float PadX = Pad(0).GetLStickXF();
	const float PadY = Pad(0).GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	//�X�e�[�g���s�B
	m_currentState->Execute();
	//���������߂�B
	Vector3 vMove = m_moveSpeed;
	vMove.y = 0.0f;
	//��]���y��������B
	Quaternion dirRot;
	float rotSpeed = CURVE_DEG;
	if (m_currentState->IsDrift()) {
		//�u���[�L���̓J�[�u���₷���B
		rotSpeed *= DRIFT_POWER;
	}
	dirRot.SetRotationDegY(Math::PI * rotSpeed * PadX * DeltaTime);
	dirRot.Apply(vMove);
	vMove.Normalize();
	//�ړ����������߂�B
	vMove = m_forward * HANDLE_WEIGHT + vMove * (1.0f - HANDLE_WEIGHT);
	//�ړ����x�����߂�B
	m_moveSpeed.x = vMove.x * m_speed;
	m_moveSpeed.z = vMove.z * m_speed;

	Vector3 moveScaler = m_moveSpeed;
	moveScaler.y = 0.0f;
	//����������B
	if (moveScaler.Length() > DEADROT_SPEED)
	{
		//�Ԃւ̉�]���v�Z�B
		vMove.Normalize();
		float angle = m_forward.Dot(vMove);
		angle = min(1.0f, max(0.0f, angle));
		float radian = acosf(angle);
		float RtoV = m_right.Dot(vMove);
		if (RtoV < 0.0f)
		{
			//�t�ɂ���B
			radian *= -1.0f;
		}
		if (fabs(radian) > 0.01f) {
			m_rotation.AddRotationY(radian);
		}
	}

	//��]����������v�Z�B
	CalcDirection();

	m_moveSpeed.y -= GRAVITY;

	m_position = m_charaCon.Execute(m_moveSpeed, DeltaTime);
}

void Car::RigidBodyMove()
{
	const float PadX = Pad(0).GetLStickXF();
	const float PadY = Pad(0).GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	m_rigidBody.GetPositionAndRotation(m_position, m_rotation);
	m_position.y -= CAR_SIZE.y / 2.0f;

	//�X�e�[�g���s�B
	m_currentState->Execute();
	//���������߂�B
	Vector3 vMove = m_moveSpeed;
	vMove.y = 0.0f;
	//��]���y��������B
	Quaternion dirRot;
	float rotSpeed = CURVE_DEG;
	if (m_currentState->IsDrift()) {
		//�u���[�L���̓J�[�u���₷���B
		rotSpeed *= DRIFT_POWER;
	}
	dirRot.SetRotationDegY(Math::PI * rotSpeed * PadX * DeltaTime);
	dirRot.Apply(vMove);
	vMove.Normalize();
	//�ړ����������߂�B
	vMove = m_forward * HANDLE_WEIGHT + vMove * (1.0f - HANDLE_WEIGHT);
	//�ړ����x�����߂�B
	m_moveSpeed.x = vMove.x * m_speed;
	m_moveSpeed.z = vMove.z * m_speed;

	//��]�����߂�B
	float radian = 0.0f;
	Vector3 moveScaler = m_moveSpeed;
	moveScaler.y = 0.0f;
	//����������B
	if (moveScaler.Length() > DEADROT_SPEED)
	{
		//�Ԃւ̉�]���v�Z�B
		vMove.Normalize();
		float angle = m_forward.Dot(vMove);
		angle = min(1.0f, max(0.0f, angle));
		radian = acosf(angle);
		float RtoV = m_right.Dot(vMove);
		if (RtoV < 0.0f)
		{
			//�t�ɂ���B
			radian *= -1.0f;
		}
		if (fabs(radian) > 0.01f) {
			m_rotation.AddRotationY(radian);
		}
	}

	//��]����������v�Z�B
	CalcDirection();

	//m_moveSpeed.y -= GRAVITY;

	m_rigidBody.SetLinerVelocity(m_moveSpeed);
	//m_rigidBody.AddForce(m_moveSpeed * DeltaTime,Vector3::Zero);
	m_rigidBody.SetAngularVelocity({ 0.0f, radian * CURVE_DEG, 0.0f });
}
