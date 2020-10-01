#include "stdafx.h"
#include "Animation.h"

namespace Engine {
	namespace {
		//DCCツールでのアニメーション上での1フレームの経過時間(s)
		const float DELTA_SEC_PER_FRAME_ON_DCC_TOOL = 1.0f / 30.0f;
	}
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