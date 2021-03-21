#pragma once

#include "CheckPointBase.h"

/// <summary>
/// チェックポイント。
/// </summary>
class CheckPointNormal :public CheckPointBase
{
public:
	CheckPointNormal();
	~CheckPointNormal();
	/// <summary>
	/// 判定を行う。
	/// </summary>
	/// <param name="checkedCon">チェックされるもの。</param>
	void CheckHit(CheckedController* checkedCon) override final;
};

