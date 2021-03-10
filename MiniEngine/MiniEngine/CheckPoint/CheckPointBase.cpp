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
	//クオータニオンから向きを求める。
	Matrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	//向きを代入する。
	m_forward = { mRot.m[2][0],mRot.m[2][1] ,mRot.m[2][2] };

	//箱の中心に座標を合わせる。
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
	//車の前方向。
	Vector3 carFront = car->GetForward();
	carFront.Normalize();
	//次のチェックポイントまでのベクトル。
	Vector3 vDir = m_nextPoint->GetPosition() - m_position;
	vDir.Normalize();
	//内積から逆走しているかを判定する。
	float angle = vDir.Dot(carFront);
	
	if (angle < REVERSE_ANGLE)
	{
		return true;
	}

	return false;
}
