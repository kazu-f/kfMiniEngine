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
	//�ڐG���Ă��邩�H
	bool isHit = false;
	//�S�[�X�g����R���g���[���[�ւ̃x�N�g���B
	Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
	ghostToCon.Normalize();
	//�S�[�X�g�̑O�������ɂ��邩�𔻒肷��B
	float angleDot = m_forward.Dot(ghostToCon);
	//�o�^�ς݁H
	auto it = std::find_if(
		m_contactedList.begin(),
		m_contactedList.end(),
		[&](ContactData& data) {
			return data.controller == checkedCon;
		}
	);
	//�S�[�X�g�Ƃ̔�����s���B
	PhysicsWorld().ContactTest(*(checkedCon->GetBody()), [&](const btCollisionObject& contactCollisionObject) {
		//�S�[�X�g�ƐڐG���Ă���B
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			isHit = true;		//�ڐG���Ă���B
			if (it == m_contactedList.end())
			{
				//�o�^����Ă��Ȃ����ߓo�^�B
				//�R���g���[���[�ƑO�����ɂ��邩�ǂ����B
				m_contactedList.push_back({ checkedCon , (angleDot >= 0.0f) });
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

			//�o�^���̑O��ƌ��݂̑O�ォ��J�E���g���グ�邩�����߂�B
			if ((angleDot < 0.0f)
				&& (it->isForward))
			{
				//�J�E���g��������B
				checkedCon->CountDownLap();
			}
			else if((angleDot >= 0.0f)
				&& (!it->isForward))
			{
				//�J�E���g���グ��B
				checkedCon->CountUpLap();
			}
			m_contactedList.erase(it);
		}
	}

}
