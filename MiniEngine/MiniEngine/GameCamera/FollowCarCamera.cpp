#include "stdafx.h"
#include "FollowCarCamera.h"
#include "Car/Car.h"

FollowCarCamera::FollowCarCamera()
{
}

FollowCarCamera::~FollowCarCamera()
{
}

bool FollowCarCamera::Start()
{
	//null�̏ꍇ�͎n�܂�Ȃ��B
	if (m_car == nullptr)
	{
		WARNING_MESSAGE_BOX("�Ԃ��w�肵�Ă��Ȃ��B");
		return false;
	}



	return true;
}

void FollowCarCamera::Update()
{
}

void FollowCarCamera::OnDestroy()
{
}
