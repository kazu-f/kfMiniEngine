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
			MessageBoxA(NULL, "AnimClip��Load�Ɏ��s�B�t�@�C���p�X�m�F�I", "Warning!!", MB_OK);
		}
	}
	void CAnimationClip::BuildKeyFramesAndAnimationEvents()
	{
		//�L�[�t���[�����̍\�z�B
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
		//�{�[���C���f�b�N�X���Ƃ̃L�[�t���[���̘A�����X�g���쐬�B
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