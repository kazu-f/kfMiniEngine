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
	//�S�[�X�g����R���g���[���[�ւ̃x�N�g���B
	Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
	ghostToCon.Normalize();
	//�S�[�X�g�̑O�������ɂ��邩�𔻒肷��B
	float angleDot = m_forward.Dot(ghostToCon);
	//�o�^�ς݁H
	auto it = std::find(
		m_contactedList.begin(),
		m_contactedList.end(),
		checkedCon
	);
	//�S�[�X�g�Ƃ̔�����s���B
	PhysicsWorld().ContactTest(*(checkedCon->GetCharaCon()), [&](const btCollisionObject& contactCollisionObject) {
		//�S�[�X�g�ƐڐG���Ă���B
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			isHit = true;		//�ڐG���Ă���B
			if (it == m_contactedList.end())
			{
				//�o�^����Ă��Ȃ����ߓo�^�B
				m_contactedList.push_back({ checkedCon});
			}

			if (angleDot < 0.0f)
			{
				//���ɂ���B
				checkedCon->SetCheckPointNum(m_checkPointNo);
			}
			else {
				//�O�������ɂ���B
				checkedCon->SetCheckPointNum(m_nextPoint->GetCheckPointNum());
			}
		}
	});
	//�ڐG���Ă��Ȃ��Ƃ��̏����B
	if (isHit == false)
	{
		//�o�^����Ă����B
		if (it != m_contactedList.end()) {
			if (angleDot < 0.0f)
			{
				//���ɂ���B
				checkedCon->CountDownLap();
			}
			else {
				//�O�������ɂ���B
				checkedCon->CountUpLap();
			}
			m_contactedList.erase(it);
		}
	}

}
