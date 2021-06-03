#include "stdafx.h"
#include "PlayerCarDriver.h"

void PlayerCarDriver::Update()
{
	//入力の更新。
	m_driverInput[enDriverAccele] = Pad(0).IsPress(enButtonA);
	m_driverInput[enDriverBrake] = Pad(0).IsPress(enButtonB);

	//アナログスティックの入力。
	m_driverHandle = Pad(0).GetLStickXF();
}
