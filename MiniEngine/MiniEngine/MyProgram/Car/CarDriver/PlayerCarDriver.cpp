#include "stdafx.h"
#include "PlayerCarDriver.h"

void PlayerCarDriver::Update()
{
	//���͂̍X�V�B
	m_driverInput[enDriverDrift] = Pad(0).IsPress(enButtonA) && Pad(0).IsPress(enButtonB);
	m_driverInput[enDriverAccele] = Pad(0).IsPress(enButtonA) ^ m_driverInput[enDriverDrift];
	m_driverInput[enDriverBrake] = Pad(0).IsPress(enButtonB) ^ m_driverInput[enDriverDrift];
	m_driverInput[enDriverIdle] = !(Pad(0).IsPress(enButtonA) || Pad(0).IsPress(enButtonB));

	//�A�i���O�X�e�B�b�N�̓��́B
	m_driverHandle = Pad(0).GetLStickXF();
}
