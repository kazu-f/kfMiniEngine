#pragma once

#include "CheckPointBase.h"

/// <summary>
/// ゴールのチェックポイント。
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
	/// 判定を行う。
	/// </summary>
	/// <param name="checkedCon">チェックされるもの。</param>
	void CheckHit(CheckedController* checkedCon) override final;
private:
	std::vector<ContactData> m_contactedList;			//接触しているものの配列。
};

