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
	//�S�[�X�g�Ƃ̔�����s���B
	PhysicsWorld().ContactTest(*(checkedCon->GetCharaCon()), [&](const btCollisionObject& contactCollisionObject) {
		//�S�[�X�g�ƐڐG���Ă���B
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			//�o�^�ς݁H
			auto it = std::find_if(
				m_contactedList.begin(), 
				m_contactedList.end(), 
				[&](SContactControllerInfo& res) { return res.controller == checkedCon; }
			);
			if (it == m_contactedList.end())
			{
				//�o�^����Ă��Ȃ����ߓo�^�B
				m_contactedList.push_back({ checkedCon, checkedCon->IsReverseRun() });
			}

			//�S�[�X�g����R���g���[���[�ւ̃x�N�g���B
			Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
			ghostToCon.Normalize();
			//�S�[�X�g�̑O�������ɂ��邩�𔻒肷��B
			float angleDot = m_forward.Dot(ghostToCon);
			if (angleDot < 0.0f)
			{
				//���ɂ���B
				checkedCon->SetCheckPointNum(m_checkPointNum);
			}
			else {
				//�O�������ɂ���B
				checkedCon->SetCheckPointNum(m_nextPoint->GetCheckPointNum());
			}
		}
	});
}
