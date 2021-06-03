#include "stdafx.h"
#include "ICarDriver.h"

void ICarDriver::ResetDriverInput()
{
	for (auto& input : m_driverInput)
	{
		input = false;
	}
	m_driverHandle = 0.0f;
}
