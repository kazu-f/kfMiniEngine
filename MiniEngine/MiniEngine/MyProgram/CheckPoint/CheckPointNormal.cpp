#include "stdafx.h"
#include "CheckPointNormal.h"

CheckPointNormal::CheckPointNormal()
{
}

CheckPointNormal::~CheckPointNormal()
{
}

void CheckPointNormal::CheckHit(CheckedController* checkedCon)
{
	//�S�[�X�g�Ƃ̔�������B
	PhysicsWorld().ContactTest(*(checkedCon->GetBody()), [&](const btCollisionObject& contactCollisionObject) {
		//�S�[�X�g�ƐڐG���Ă���B
		if (m_ghostObj.IsSelf(contactCollisionObject)) {
			//�S�[�X�g����R���g���[���[�ւ̃x�N�g���B
			Vector3 ghostToCon = checkedCon->GetPosition() - m_position;
			ghostToCon.Normalize();
			//�S�[�X�g�̑O�������ɂ��邩�𔻒肷��B
			float angleDot = m_forward.Dot(ghostToCon);
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
}
