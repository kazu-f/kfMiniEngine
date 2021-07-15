#include "stdafx.h"
#include "CarMoveController.h"
#include "Car.h"
#include "State/ICarState.h"
#include "State/CarStateIdle.h"
#include "State/CarStateAccele.h"
#include "State/CarStateBrake.h"
#include "State/CarStateDrift.h"
#include "CarDriver/ICarDriver.h"

namespace {

	const float GRAVITY = 980.0f;			//�d�͉����x�B
	const float HANDLE_WEIGHT = 0.3f;		//�n���h���̌����B
	const float CURVE_DEG = 15.0f;			//�J�[�u�̊p�x�B
	const float DRIFT_POWER = 1.4f;			//�h���t�g�̋����B
	const float WALLHIT_MINSPEED = 2400.0f;	//�ǂɐڐG���Ă���Ƃ��̍Œᑬ�x�B
	
	const float DEADROT_STARTSPEED = 1800.0f;		//���x�����ȉ��Ȃ��]���Ȃ��B
	const float DEADROT_ENDSPEED = 100.0f;		//���x�����ȉ��Ȃ��]���Ȃ��B

}

const float CarMoveController::MOVE_COEFFICIENT = 60.0f;

CarMoveController::CarMoveController()
{
	//�X�e�[�g���������B
	m_carStates[EnCarState::enStateIdle] = std::make_unique<CarStateIdle>(this);
	m_carStates[EnCarState::enStateAccele] = std::make_unique<CarStateAccele>(this);
	m_carStates[EnCarState::enStateBrake] = std::make_unique<CarStateBrake>(this);
	m_carStates[EnCarState::enStateDrift] = std::make_unique<CarStateDrift>(this);
}

CarMoveController::~CarMoveController()
{
}

void CarMoveController::Init(const Vector3& position, const Quaternion& rotation)
{
	m_position = position;
	m_rotation = rotation;
	//�L�����R���̏������B
	m_charaCon.Init(
		50.0f,
		50.0f,
		m_position
	);
}

bool CarMoveController::MoveCar()
{
	if (m_currentState == nullptr) return false;
	if (m_carDriver == nullptr) return false;
	const float HandlePower = m_carDriver->GetDriverHandle();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	//�X�e�[�g���s�B
	m_currentState->Execute();

	//���������߂�B
	//��]���y��������B
	Quaternion dirRot;
	float rotSpeed = CURVE_DEG * HandlePower;
	if (m_currentState->IsDecelerate()) {
		//�������̓J�[�u���₷���B
		rotSpeed *= DRIFT_POWER;
	}

	float speedWeight = max(0.0f, min(1.0f, (DEADROT_ENDSPEED - m_speed) / (DEADROT_ENDSPEED - DEADROT_STARTSPEED)));
	dirRot.SetRotationDegY(Math::PI * rotSpeed * DeltaTime * speedWeight);
	m_rotation.Multiply(dirRot);

	//��]����������v�Z�B
	CalcDirection();

	//�ړ����x�����߂�B
	if (m_charaCon.IsHitWall())
	{
		if (m_speed > WALLHIT_MINSPEED)
		{
			m_speed *= 0.995f;
			m_speed = max(m_speed, WALLHIT_MINSPEED);
		}
	}

	m_moveSpeed.x = m_forward.x * m_speed;
	m_moveSpeed.z = m_forward.z * m_speed;

	m_moveSpeed.y -= GRAVITY;

	m_position = m_charaCon.Execute(m_moveSpeed, DeltaTime);

	return true;
}

void CarMoveController::ChangeState(EnCarState state)
{
	//�ω��Ȃ��B
	if (m_currentState == m_carStates[state].get())
	{
		return;
	}

	if (m_currentState != nullptr)
	{
		//���݃X�e�[�g�̎��㏈���B
		m_currentState->Leave();
	}
	//�X�e�[�g��؂�ւ��B
	m_currentState = m_carStates[state].get();
	//���O�����B
	m_currentState->Enter();
}

void CarMoveController::AddDicceleration(const float diccele)
{
	float MinSpeed = 0.0f;
	if (m_currentState->IsDrift())
	{
		MinSpeed = min(m_speed * 0.9f, MAX_SPEED * (3.0f / 4.0f));
	}
	m_speed = max(MinSpeed, m_speed - (diccele * MOVE_COEFFICIENT * GameTime().GetFrameDeltaTime()));
}

void CarMoveController::CalcDirection()
{
	//�N�I�[�^�j�I��������������߂�B
	Matrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	//������������B
	m_forward = { mRot.m[2][0],mRot.m[2][1] ,mRot.m[2][2] };
	m_right = { mRot.m[0][0],mRot.m[0][1] ,mRot.m[0][2] };
	m_up = { mRot.m[1][0],mRot.m[1][1] ,mRot.m[1][2] };
}
