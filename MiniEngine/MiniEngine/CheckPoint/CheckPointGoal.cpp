#include "stdafx.h"
#include "CheckPointGoal.h"

CheckPointGoal::CheckPointGoal()
{
}

CheckPointGoal::~CheckPointGoal()
{
}

void CheckPointGoal::CheckCharaconHit(CheckedController* checkedCon)
{
	//ゴーストとの判定を行う。
	PhysicsWorld().ContactTest(*(checkedCon->GetCharaCon()), [&](const btCollisionObject& contactCollisionObject) {
		//ゴーストと接触している。
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			//登録済み？
			auto it = std::find_if(
				m_contactedList.begin(), 
				m_contactedList.end(), 
				[&](SContactControllerInfo& res) { return res.controller == checkedCon; }
			);
			if (it == m_contactedList.end())
			{
				//登録されていないため登録。
				m_contactedList.push_back({ checkedCon, checkedCon->IsReverseRun() });
			}

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
