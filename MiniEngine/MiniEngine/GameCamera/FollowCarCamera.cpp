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
		ENGINE_MESSAGE_BOX(
			"�Ԃ̃A�h���X��null�ł����B\n"
			"SetTargetCar�֐��Ŏw�肵�Ă��������B\n"
		);
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
