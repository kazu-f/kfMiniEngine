#include "stdafx.h"
#include "CheckPointNormal.h"

CheckPointNormal::CheckPointNormal()
{
}

CheckPointNormal::~CheckPointNormal()
{
}

void CheckPointNormal::CheckCharaconHit(CheckedController* checkedCon)
{
	//ゴーストとの判定を取る。
	PhysicsWorld().ContactTest(*(checkedCon->GetCharaCon()), [&](const btCollisionObject& contactCollisionObject) {
		//ゴーストと接触している。
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			//ゴーストからコントローラーへのベクトル。
			Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
			ghostToCon.Normalize();
			//ゴーストの前方向側にいるかを判定する。
			float angleDot = m_forward.Dot(ghostToCon);
			if (angleDot < 0.0f)
			{
				//後ろにいる。
				checkedCon->SetCheckPointNum(m_checkPointNum);
			}
			else {
				//前方向側にいる。
				checkedCon->SetCheckPointNum(m_nextPoint->GetCheckPointNum());
			}
		}
		});
}
