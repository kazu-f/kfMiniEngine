#include "stdafx.h"
#include "FollowCarCamera.h"
#include "Car/Car.h"

namespace {
	const Vector3 CAMERA_DISTANCE = { 0.0f,150.0f,-800.0f };		//�J�����̍ŏ��̃^�[�Q�b�g����̈ʒu�B
	const float CAMERA_FAR = 250000.0f;							//�J�����̉����ʁB
	const float TARGET_HIGHT = 80.0f;							//�J�����̃^�[�Q�b�g�̍��W����̍����B
	const float FOLLOW_WEIGHT = 0.7f;							//�Ǐ]���鋭��(0.0�`1.0)�B1.0�ɋ߂��قǒǏ]�������B
	const float MAX_LENGTH = 1000.0f;							//�Ǐ]����ۂ̍ő勗���B
	const float MIN_LENGTH = 600.0f;							//�Ǐ]���鎞�̍ŏ������B
}

FollowCarCamera::FollowCarCamera()
{
}

FollowCarCamera::~FollowCarCamera()
{
}

bool FollowCarCamera::Start()
{
	//null�̏ꍇ�͎n�܂�Ȃ��B
	if (m_car == nullptr)
	{
		ENGINE_LOG(
			"FollowCarCamera::Start():"
			"�Ԃ̃A�h���X��null�ł����B"
			"SetTargetCar�֐��Ŏw�肵�Ă��������B"
		);
		return false;
	}

	//�����ʂ̐ݒ�B
	MainCamera().SetFar(CAMERA_FAR);

	m_target = m_car->GetPosition();	//�^�[�Q�b�g�ƂȂ�Ԃ̍��W���擾�B
	m_target.y += TARGET_HIGHT;			//������������B
	m_position = m_target + CAMERA_DISTANCE;

	MainCamera().SetPosition(m_position);
	MainCamera().SetTarget(m_target);


	return true;
}

void FollowCarCamera::Update()
{
	//�Ԃ̃A�h���X���Ȃ��B
	if (m_car == nullptr)
	{
		return;
	}
	//�Ԃ���������null������B
	if (m_car->IsDead())
	{
		m_car = nullptr;
		return;
	}
	//XZ���ʏ�̃x�N�g�������߂�B
	Vector3 toCameraPosXZ = m_position - m_target;
	//�x�N�g����Y�����͎��o���Ă����B
	float toCameraPosY = toCameraPosXZ.y;
	toCameraPosXZ.y = 0.0f;
	//�x�N�g���̒������L�^���Ă����B
	//float toCameraPosXZLen = toCameraPosXZ.Length();
	toCameraPosXZ.Normalize();		//���K���B

	//�^�[�Q�b�g�̐V�������W���擾�B
	Vector3 targetPos = m_car->GetPosition();
	targetPos.y += TARGET_HIGHT;

	//�V���������_�ƌ��݂̃J�����̎��_���g����XZ���ʂł̃x�N�g�������߂�B
	Vector3 toNewCameraPos = m_position - targetPos;
	toNewCameraPos.y = 0.0f;
	//�������L�^����B
	float toNewCameraPosXZLen = toNewCameraPos.Length();
	toNewCameraPos.Normalize();

	//���_�̈ʒu���v�Z���Ă����B
	toNewCameraPos = toNewCameraPos * FOLLOW_WEIGHT + toCameraPosXZ * (1.0f - FOLLOW_WEIGHT);
	toNewCameraPos.Normalize();			//���K���B
	//�΂˃J�����I�ȋ����̂��߂̋������v�Z�B
	float finalLen = toNewCameraPosXZLen * FOLLOW_WEIGHT + MIN_LENGTH * (1.0f - FOLLOW_WEIGHT);
	finalLen = min(MAX_LENGTH, max(MIN_LENGTH, finalLen));
	//�x�N�g���̒������|���Ď��_�܂ł̃x�N�g�����o���B
	toNewCameraPos *= finalLen;
	//������߂��B
	toNewCameraPos.y = toCameraPosY;

	//���W���J�����ɐݒ肷��B
	m_position = targetPos + toNewCameraPos;
	m_target = targetPos;

	//�J�����̍��W���X�V�B
	MainCamera().SetTarget(m_target);
	MainCamera().SetPosition(m_position);
}

void FollowCarCamera::OnDestroy()
{
}
