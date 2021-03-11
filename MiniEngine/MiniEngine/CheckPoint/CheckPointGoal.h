#pragma once

#include "CheckPointBase.h"

/// <summary>
/// ゴールのチェックポイント。
/// </summary>
class CheckPointGoal : public CheckPointBase
{
public:
	CheckPointGoal();
	~CheckPointGoal();
	/// <summary>
	/// 判定を行う。
	/// </summary>
	/// <param name="checkedCon">チェックされるもの。</param>
	void CheckCharaconHit(CheckedController* checkedCon) override final;
private:
	std::vector<CheckedController*> m_contactedList;			//接触しているものの配列。
};

