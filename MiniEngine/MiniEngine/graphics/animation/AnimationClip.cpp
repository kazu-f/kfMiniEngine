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
			return;
		}
	}
	void CAnimationClip::BuildKeyFramesAndAnimationEvents()
	{
		//アニメーションイベントの構築。
		{
			auto numAnimEvent = m_tkaFile.GetNumAnimationEvent();
			if (numAnimEvent > 0)
			{
				m_animationEvent = std::make_unique<CAnimationEvent[]>(numAnimEvent);
				int eventNo = 0;
				m_tkaFile.QueryAnimationEvents([&](const TkaFile::AnimationEvent& animEvent) {
					static wchar_t wEventName[256];
					//イベント名をワイド型文字列に変換。
					mbstowcs(wEventName, animEvent.eventName.c_str(), 255);
					m_animationEvent[eventNo].SetInvokeTime(animEvent.invokeTime);
					m_animationEvent[eventNo].SetEventName(wEventName);
					eventNo++;
					});
			}
		}

		//キーフレーム情報の構築。
		{
			m_keyframes.reserve(m_tkaFile.GetNumKeyFrame());
			m_tkaFile.QueryKeyFrames([&](const TkaFile::KeyFrame& tkaKeyFrame) {
			auto keyframe = std::make_unique<KeyFrame>();
			//キーフレームの情報のコピー。
			keyframe->boneIndex = tkaKeyFrame.boneIndex;
			keyframe->transform = Matrix::Identity;
			keyframe->time = tkaKeyFrame.time;
			for (auto j = 0; j < 4; j++) {
				//ベクトルから行列に変換。
				keyframe->transform.m[j][0] = tkaKeyFrame.transform[j].x;
				keyframe->transform.m[j][1] = tkaKeyFrame.transform[j].y;
				keyframe->transform.m[j][2] = tkaKeyFrame.transform[j].z;
			}
			//キーフレームリストに積む。
			m_keyframes.push_back(std::move(keyframe));
			});
		}
		//ボーンインデックスごとのキーフレームの連結リストを作成。
		{
			m_keyFramePtrListArray.resize(MAX_BONE);
			for (auto& keyframe : m_keyframes) {
				//ボーン番号ごとに割り振る。
				m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe.get());
				if (m_topBoneKeyFrameList == nullptr) {
					//一番上のボーンのキーフレームリスト。
					m_topBoneKeyFrameList = &m_keyFramePtrListArray[keyframe->boneIndex];
				}
			}
		}
	}
}