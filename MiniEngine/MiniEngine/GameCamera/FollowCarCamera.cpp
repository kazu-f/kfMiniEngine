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
		WARNING_MESSAGE_BOX("車を指定していない。");
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
