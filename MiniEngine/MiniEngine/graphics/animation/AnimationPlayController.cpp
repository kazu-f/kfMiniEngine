/// <summary>
/// アニメーション再生コントローラ。
/// </summary>

#include "stdafx.h"
#include "AnimationPlayController.h"

namespace Engine {
	void CAnimationPlayController::Init(Skeleton* skeleton, int footStepBoneNo)
	{
		m_footstepBoneNo = footStepBoneNo;
		int numBones = skeleton->GetNumBones();
		//ボーン行列をバシッと確保。
		m_boneMatrix.resize(numBones);
		m_skeleton - skeleton;
	}
	void CAnimationPlayController::InvokeAnimationEvent(CAnimation* animation)
	{
		auto& animEventArray = m_animationClip->GetAnimationEvent();
		for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
			if (m_time > animEventArray[i].IsInvoked() == false) {
				//アニメーションの起動時間を過ぎていて、まだイベントを起動していない。
				//アニメーションの処理まだ//////////
			}
		}
	}
	void CAnimationPlayController::StartLoop()
	{
		//値を初期化。
		m_footstepPos = Vector3::Zero;
		m_currentKeyFrameNo = 0;
		m_time = 0.0f;
		//アニメーションイベントを全て未発生にする。
		auto& animEventArray = m_animationClip->GetAnimationEvent();
		for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++){
			animEventArray[i].SetInvokedFlag(false);
		}
	}
	void CAnimationPlayController::CalcBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix)
	{
		//ワールド行列を計算する。
		auto& mBoneInRootSpace = m_boneMatrix[bone.GetNo()];
		Matrix localMatrix = m_boneMatrix[bone.GetNo()];
		//親の行列とローカル行列を乗算して、ワールド行列を計算する。
		mBoneInRootSpace = localMatrix * parentMatrix;

		//子供のワールド行列も計算する。
		for (auto& childBone : bone.GetChildren()){
			CalcBoneMatrixInRootBoneSpace(*childBone, mBoneInRootSpace);
		}
	}
	void CAnimationPlayController::SamplingBoneMatrixFromAnimationClip()
	{
		//ボーン毎のキーフレームリスト。
		const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
		for (const auto& keyFrameList : keyFramePtrListArray) {
			//ボーンにキーフレームがない？
			if (keyFrameList.size() == 0) {
				continue;
			}
			//現在再生中のキーフレームを取ってくる。
			KeyFrame* keyframe = keyFrameList.at(m_currentKeyFrameNo);
			//再生中のキーフレーム番号でのボーン行列取得。
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		}
	}
	void CAnimationPlayController::CalcBoneMatrixInRootBoneSpace()
	{
		int numBone = m_skeleton->GetNumBones();
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			//ルートの骨を検索する。
			auto bone = m_skeleton->GetBone(boneNo);
			//親がいるなら無視
			if (bone->GetParentBoneNo() != -1) {
				continue;
			}
			//ボーン毎のワールド行列の計算を開始する。
			CalcBoneMatrixInRootBoneSpace(*bone, g_matIdentity);
		}
	}
	void CAnimationPlayController::SamplingDataValueFootstepBone()
	{
		if (m_currentKeyFrameNoLastFrame == m_currentKeyFrameNo) {
			//キーフレームが進んでいない。
			return;
		}
		if (m_footstepBoneNo == -1) {
			//footstepボーンが存在しない。
			return;
		}
		int numBone = m_skeleton->GetNumBones();

		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			auto bone = m_skeleton->GetBone(boneNo);
			if (m_footstepBoneNo == bone->GetNo()) {
				auto mat = m_boneMatrix[bone->GetNo()];
				Vector3 footstepBonePos;
				footstepBonePos.x = mat.m[3][0];
				footstepBonePos.y = mat.m[3][1];
				footstepBonePos.z = mat.m[3][2];
				//このフレームでのfootstepの移動量の計算。
				m_footstepDeltaValue = footstepBonePos - m_footstepPos;
				//このフレームでのfootstepの座標を更新する。
				m_footstepPos = footstepBonePos;
				break;
			}
		}
	}
	void CAnimationPlayController::SubtractFootstepBonePosFromAllBone()
	{
		if (m_footstepBoneNo == -1) {
			//footstepボーンが存在しない。
			return;
		}
		int numBone = m_skeleton->GetNumBones();

		//全てのボーンからfootstepの座標を引いていく。
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			auto bone = m_skeleton->GetBone(boneNo);
			m_boneMatrix[bone->GetNo()].m[3][0] -= m_footstepPos.x;
			m_boneMatrix[bone->GetNo()].m[3][1] -= m_footstepPos.y;
			m_boneMatrix[bone->GetNo()].m[3][2] -= m_footstepPos.z;
		}
	}
	void CAnimationPlayController::ProgressKeyframeNo(float deltaTime)
	{
		//1フレーム前のキーフレーム番号を記録。
		m_currentKeyFrameNoLastFrame = m_currentKeyFrameNo;

		const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();

		//補完時間も進めていく。
		m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);

		while (true) {
			if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
				//終端まで行った。
				if (m_animationClip->IsLoop()) {
					//ループ。
					StartLoop();
				}
				else {
					//ワンショット再生。
					m_currentKeyFrameNo--;
					m_isPlaying = false;	//再生終わり。
				}
				break;
			}
			if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
				//終わり。
				break;
			}
			//次へ。
			m_currentKeyFrameNo++;
		}

	}
	void CAnimationPlayController::Update(float deltaTime, CAnimation* animation)
	{
		if (m_animationClip == nullptr) {
			//アニメーションクリップが存在しない。
			MessageBoxA(NULL, "アニメーションクリップがnullです。", "Warning!!", MB_OK);
			return;
		}
		
		//時間を経過させる。
		m_time += deltaTime;
		
		//アニメーションイベントの発生。
		InvokeAnimationEvent(animation);
		
		//キーフレーム番号を進める。
		ProgressKeyframeNo(deltaTime);
		
		//ボーン行列をアニメーションクリップからサンプリングしていく。
		SamplingBoneMatrixFromAnimationClip();

		//親の骨座標系になっているボーン行列をルートのボーン空間に変換していく。
		CalcBoneMatrixInRootBoneSpace();

		//footstepボーンの移動量を取得する。
		SamplingDataValueFootstepBone();

		//footstepボーンの移動量を全体の骨から減算する。
		SubtractFootstepBonePosFromAllBone();

		//アニメーション再生した印をスケルトンにつける。
		m_skeleton->SetMarkPlayAnimation();

	}
}