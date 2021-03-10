#include "stdafx.h"
#include "CheckPointBase.h"

namespace {
	const float REVERSE_ANGLE = -0.7f;
	const Vector3 CHECKPOINT_BOXSIZE = {2700.0f,500.0f,500.0f};
}

CheckPointBase::CheckPointBase()
{
}

CheckPointBase::~CheckPointBase()
{
}

void CheckPointBase::Init()
{
	//�N�I�[�^�j�I��������������߂�B
	Matrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	//������������B
	m_forward = { mRot.m[2][0],mRot.m[2][1] ,mRot.m[2][2] };

	//���̒��S�ɍ��W�����킹��B
	Vector3 pos = m_position;
	pos.z += CHECKPOINT_BOXSIZE.z / 2.0f;

	m_ghostObj.CreateBox(
		pos,
		m_rotation,
		CHECKPOINT_BOXSIZE
		);
}

bool CheckPointBase::CheckReverseRun(CheckedController* car)
{
	//�Ԃ̑O�����B
	Vector3 carFront = car->GetForward();
	carFront.Normalize();
	//���̃`�F�b�N�|�C���g�܂ł̃x�N�g���B
	Vector3 vDir = m_nextPoint->GetPosition() - m_position;
	vDir.Normalize();
	//���ς���t�����Ă��邩�𔻒肷��B
	float angle = vDir.Dot(carFront);
	
	if (angle < REVERSE_ANGLE)
	{
		return true;
	}

	return false;
}
