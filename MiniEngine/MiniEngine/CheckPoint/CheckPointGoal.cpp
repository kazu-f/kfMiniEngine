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
	bool isHit = false;
	//ゴーストからコントローラーへのベクトル。
	Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
	ghostToCon.Normalize();
	//ゴーストの前方向側にいるかを判定する。
	float angleDot = m_forward.Dot(ghostToCon);
	//登録済み？
	auto it = std::find(
		m_contactedList.begin(),
		m_contactedList.end(),
		checkedCon
	);
	//ゴーストとの判定を行う。
	PhysicsWorld().ContactTest(*(checkedCon->GetCharaCon()), [&](const btCollisionObject& contactCollisionObject) {
		//ゴーストと接触している。
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			isHit = true;		//接触している。
			if (it == m_contactedList.end())
			{
				//登録されていないため登録。
				m_contactedList.push_back({ checkedCon});
			}

			if (angleDot < 0.0f)
			{
				//後ろにいる。
				checkedCon->SetCheckPointNum(m_checkPointNo);
			}
			else {
				//前方向側にいる。
				checkedCon->SetCheckPointNum(m_nextPoint->GetCheckPointNum());
			}
		}
	});
	//接触していないときの処理。
	if (isHit == false)
	{
		//登録されていた。
		if (it != m_contactedList.end()) {
			if (angleDot < 0.0f)
			{
				//後ろにいる。
				checkedCon->CountDownLap();
			}
			else {
				//前方向側にいる。
				checkedCon->CountUpLap();
			}
			m_contactedList.erase(it);
		}
	}

}
