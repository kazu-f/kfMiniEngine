#include "stdafx.h"
#include "Animation.h"

namespace Engine {
	namespace {
		//DCC�c�[���ł̃A�j���[�V������ł�1�t���[���̌o�ߎ���(s)
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