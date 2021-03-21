#pragma once

#include "CheckPointBase.h"

/// <summary>
/// �S�[���̃`�F�b�N�|�C���g�B
/// </summary>
class CheckPointGoal : public CheckPointBase
{
	struct ContactData {
		CheckedController* controller = nullptr;
		bool isForward = false;
	};
public:
	CheckPointGoal();
	~CheckPointGoal();
	/// <summary>
	/// ������s���B
	/// </summary>
	/// <param name="checkedCon">�`�F�b�N�������́B</param>
	void CheckHit(CheckedController* checkedCon) override final;
private:
	std::vector<ContactData> m_contactedList;			//�ڐG���Ă�����̂̔z��B
};

