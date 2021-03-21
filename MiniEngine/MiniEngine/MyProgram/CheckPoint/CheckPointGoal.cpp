#include "stdafx.h"
#include "CheckPointGoal.h"

CheckPointGoal::CheckPointGoal()
{
}

CheckPointGoal::~CheckPointGoal()
{
}

void CheckPointGoal::CheckHit(CheckedController* checkedCon)
{
	//接触しているか？
	bool isHit = false;
	//ゴーストからコントローラーへのベクトル。
	Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
	ghostToCon.Normalize();
	//ゴーストの前方向側にいるかを判定する。
	float angleDot = m_forward.Dot(ghostToCon);
	//登録済み？
	auto it = std::find_if(
		m_contactedList.begin(),
		m_contactedList.end(),
		[&](ContactData& data) {
			return data.controller == checkedCon;
		}
	);
	//ゴーストとの判定を行う。
	PhysicsWorld().ContactTest(*(checkedCon->GetBody()), [&](const btCollisionObject& contactCollisionObject) {
		//ゴーストと接触している。
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			isHit = true;		//接触している。
			if (it == m_contactedList.end())
			{
				//登録されていないため登録。
				//コントローラーと前方向にいるかどうか。
				m_contactedList.push_back({ checkedCon , (angleDot >= 0.0f) });
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

			//登録時の前後と現在の前後からカウントを上げるかを決める。
			if ((angleDot < 0.0f)
				&& (it->isForward))
			{
				//カウントを下げる。
				checkedCon->CountDownLap();
			}
			else if((angleDot >= 0.0f)
				&& (!it->isForward))
			{
				//カウントを上げる。
				checkedCon->CountUpLap();
			}
			m_contactedList.erase(it);
		}
	}

}
