#pragma once

#include "CheckPointBase.h"

/// <summary>
/// ゴールのチェックポイント。
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
	/// 判定を行う。
	/// </summary>
	/// <param name="checkedCon">チェックされるもの。</param>
	void CheckCharaconHit(CheckedController* checkedCon) override final;
private:
	std::vector<SContactControllerInfo> m_contactedList;			//接触しているものの配列。
};

