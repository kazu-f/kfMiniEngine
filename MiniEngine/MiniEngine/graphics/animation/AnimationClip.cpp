#include "stdafx.h"
#include "AnimationClip.h"

namespace Engine {
	CAnimationClip::~CAnimationClip()
	{
	}
	void CAnimationClip::Load(const char* filePath)
	{
		m_tkaFile.Load(filePath);
		if (m_tkaFile.IsLoaded() == false)
		{
			MessageBoxA(NULL, "AnimClipのLoadに失敗。ファイルパス確認！", "Warning!!", MB_OK);
		}
	}
	void CAnimationClip::BuildKeyFramesAndAnimationEvents()
	{
		//キーフレーム情報の構築。
		{
			m_keyframes.reserve(m_tkaFile.GetNumKeyFrame());
			m_tkaFile.QueryKeyFrames([&](const TkaFile::KeyFrame& tkaKeyFrame) {
			auto keyframe = std::make_unique<KeyFrame>();
			keyframe->boneIndex = tkaKeyFrame.boneIndex;
			keyframe->transform = Matrix::Identity;
			keyframe->time = tkaKeyFrame.time;
			for (auto j = 0; j < 4; j++) {
				keyframe->transform.m[j][0] = tkaKeyFrame.transform[j].x;
				keyframe->transform.m[j][1] = tkaKeyFrame.transform[j].y;
				keyframe->transform.m[j][2] = tkaKeyFrame.transform[j].z;
			}
			m_keyframes.push_back(std::move(keyframe));
			});
		}
		//ボーンインデックスごとのキーフレームの連結リストを作成。
		{
			m_keyFramePtrListArray.resize(MAX_BONE);
			for (auto& keyframe : m_keyframes) {
				m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe.get());
				if (m_topBoneKeyFrameList == nullptr) {
					m_topBoneKeyFrameList = &m_keyFramePtrListArray[keyframe->boneIndex];
				}
			}
		}
	}
}