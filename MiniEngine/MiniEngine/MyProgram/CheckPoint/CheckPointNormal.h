#pragma once

#include "CheckPointBase.h"

/// <summary>
/// �`�F�b�N�|�C���g�B
/// </summary>
class CheckPointNormal :public CheckPointBase
{
public:
	CheckPointNormal();
	~CheckPointNormal();
	/// <summary>
	/// ������s���B
	/// </summary>
	/// <param name="checkedCon">�`�F�b�N�������́B</param>
	void CheckHit(CheckedController* checkedCon) override final;
};

