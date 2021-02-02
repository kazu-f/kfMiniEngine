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
	//nullの場合は始まらない。
	if (m_car == nullptr)
	{
		ENGINE_MESSAGE_BOX(
			"車のアドレスがnullでした。\n"
			"SetTargetCar関数で指定してください。\n"
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
