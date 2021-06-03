#include "stdafx.h"
#include "CheckedController.h"

CheckedController::CheckedController()
{
}

CheckedController::~CheckedController()
{
}

void CheckedController::Init(CRigidBody* rb, Vector3& pos, Quaternion& rot)
{
	m_rigidBody = rb;
	m_position = pos;
	m_rotation = rot;
}

void CheckedController::Update(Vector3& pos, Quaternion& rot)
{
	m_position = pos;
	m_rotation = rot;
	CalcDirection();
}

void CheckedController::CalcDirection()
{
	//�N�I�[�^�j�I��������������߂�B
	Matrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	//������������B
	m_forward = { mRot.m[2][0],mRot.m[2][1] ,mRot.m[2][2] };
	m_right = { mRot.m[0][0],mRot.m[0][1],mRot.m[0][2] };
}
