#pragma once

#include "CheckPointBase.h"

/// <summary>
/// �S�[���̃`�F�b�N�|�C���g�B
/// </summary>
class CheckPointGoal : public CheckPointBase
{
	struct SContactControllerInfo
	{
		CheckedController* controller = nullptr;
		bool isReverse = false;
	};
public:
	CheckPointGoal();
	~CheckPointGoal();
	/// <summary>
	/// ������s���B
	/// </summary>
	/// <param name="checkedCon">�`�F�b�N�������́B</param>
	void CheckCharaconHit(CheckedController* checkedCon) override final;
private:
	std::vector<SContactControllerInfo> m_contactedList;			//�ڐG���Ă�����̂̔z��B
};

