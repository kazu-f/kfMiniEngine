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
	void CheckCharaconHit(CheckedController* checkedCon) override final;
};

