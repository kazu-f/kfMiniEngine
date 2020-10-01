#include "stdafx.h"
#include "Animation.h"

namespace Engine {
	CAnimation::CAnimation()
	{
	}
	CAnimation::~CAnimation()
	{
	}
	void CAnimation::Init(Skeleton & skeleton, const std::vector<std::unique_ptr<CAnimationClip >>& animClips)
	{
	}
	void CAnimation::Progress(float deltaTime)
	{
	}
	void CAnimation::UpdateLocalPose(float deltaTime)
	{
	}
	void CAnimation::UpdateGlobalPose()
	{
	}
}