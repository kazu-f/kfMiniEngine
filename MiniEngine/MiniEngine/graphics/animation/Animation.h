#pragma once

/// <summary>
/// アニメーション。
/// </summary>

#include "AnimationClip.h"
#include "AnimationPlayController.h"

namespace Engine {
	class Skeleton;
	using AnimationEventListener = std::function<void(const wchar_t* clipname, const wchar_t* eventName)>;

	class CAnimation
	{
	public:
		CAnimation();
		~CAnimation();
		/// <summary>
		/// 初期化済みか判定する。
		/// </summary>
		/// <returns>trueで初期化済み。</returns>
		bool IsInited()const
		{
			return m_isInited;
		}
		/// <summary>
		/// アニメーションの初期化処理。
		/// </summary>
		/// <param name="skinModel">アニメーションさせるモデル。</param>
		/// <param name="animClipList">アニメーションクリップのリスト。</param>
		/// <param name="animClipNum">アニメーションクリップの数。</param>
		void Init(Skeleton & skeleton, const std::vector<std::unique_ptr<CAnimationClip >>& animClips);
		/// <summary>
		/// アニメーションの再生。
		/// </summary>
		/// <param name="clipNo">アニメーションクリップ番号。</param>
		/// <param name="interpolateTime">補完時間。</param>
		void Play(int clipNo, float interpolateTime = 0.0f)
		{
			if (clipNo < m_animationClips.size()) {
				PlayCommon(m_animationClips[clipNo], interpolateTime);
			}
		}
		/// <summary>
		/// アニメーションクリップのループフラグを設定する。
		/// </summary>
		/// <param name="clipName">アニメーションクリップの名前</param>
		/// <param name="flag">フラグ</param>
		void SetAnimationClipLoopFlag(const wchar_t* clipName, bool flag)
		{
			auto it = std::find_if(
				m_animationClips.begin(),
				m_animationClips.end(),
				[clipName](auto & clip) { return clip->GetName() == clipName; }
			);
			if (it == m_animationClips.end())
			{
				//見つからなかった。
				return;
			}
			(*it)->SetLoopFlag(flag);
		}
		/// <summary>
		/// アニメーションの再生中か？
		/// </summary>
		/// <returns></returns>
		bool IsPlaying() const
		{
			int lastIndex = GetLastAnimationControllerIndex();
			return m_animationPlayController[lastIndex].IsPlaying();
		}
		/// <summary>
		/// アニメーションを進める。
		/// </summary>
		/// <remarks>
		/// エンジン内部で使用される関数。
		/// 外部からは使用しない！
		/// </remarks>
		/// <param name="deltaTime">アニメーションを進める時間(s)</param>
		void Progress(float deltaTime);
		/// <summary>
		/// アニメーションイベントリスナーを登録。
		/// </summary>
		/// <param name="eventListener">登録したいイベントリスナー。</param>
		void AddAnimationEventListener(AnimationEventListener eventListener)
		{
			m_animationEventListeners.push_back(eventListener);
		}
		/// <summary>
		/// ワールド空間でのフットステップの移動量を計算する。
		/// </summary>
		/// <remarks>
		/// フットステップの移動量は、モデルのルートからの相対移動量。
		/// そのため、ワールド空間に変換するのに平行移動量は不要。
		/// モデルの回転と拡大率のみ指定する。
		/// </remarks>
		/// <param name="rotation"></param>
		/// <param name="scale"></param>
		/// <returns></returns>
		Vector3 CalsFootstepDeltaInWorldSpace(Quaternion rotation, Vector3 scale)const;
	private:
		/// <summary>
		/// アニメーションの再生
		/// </summary>
		/// <param name="nectClip"></param>
		/// <param name="interpolateTime"></param>
		void PlayCommon(CAnimationClip * nextClip, float interpolateTime)
		{
			if (nextClip->IsLoaded() == false)
			{
				//アニメーションクリップがロードされていない。
				MessageBoxA(NULL, "アニメーションクリップがロードされていない。", "Warning!!", MB_OK);
				return;
			}
			int index = GetLastAnimationControllerIndex();
			
			if (m_animationPlayController[index].GetAnimClip() == nextClip) {
				//同じアニメーションが流れる。
				return;
			}
			if (interpolateTime == 0.0f) {
				//補完無し。
				m_numAnimationPlayController = 1;
			}
			else {
				//補完あり。
				m_numAnimationPlayController++;
			}
			index = GetLastAnimationControllerIndex();
			m_animationPlayController[index].ChangeAnimationClip(nextClip);
			m_animationPlayController[index].SetInterpolateTime(interpolateTime);
			m_interpolateTime = 0.0f;				//補完時間を0に
			m_interpolateTimeEnd = interpolateTime;	//補完終了時間を設定。

		}
		/// <summary>
		/// ローカルポーズの更新。
		/// </summary>
		/// <param name="deltaTime">アニメーションを進める時間(s)</param>
		void UpdateLocalPose(float deltaTime);
		/// <summary>
		/// グローバルポーズの更新。
		/// </summary>
		void UpdateGlobalPose();
	private:
		/// <summary>
		/// 最終ポーズになるアニメーションのリングバッファ上でのインデックスを取得。
		/// </summary>
		int GetLastAnimationControllerIndex()const
		{
			return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
		}
		/// <summary>
		/// アニメーションコントローラのリングバッファ上でのインデックスを取得。
		/// </summary>
		/// <param name="startIndex">開始インデックス。</param>
		/// <param name="localIndex">ローカルインデックス。</param>
		int GetAnimationControllerIndex(int startIndex, int localIndex)const
		{
			return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
		}

	private:
		static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;	//アニメーションコントローラの数。
		std::vector<CAnimationClip*>	m_animationClips;		//アニメーションクリップの配列。
		Skeleton* m_skeleton = nullptr;							//アニメーションを適用するスケルトン。
		CAnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];		//アニメーションプレイコントローラ。
		int m_numAnimationPlayController = 0;					//現在使用中のアニメーション再生コントローラの数。
		int m_startAnimationPlayController = 0;					//アニメーションコントローラの開始インデックス。
		float m_interpolateTime = 0.0f;							//補完時間。
		float m_interpolateTimeEnd = 0.0f;						//補完終了時間。？
		bool m_isInterpolate = false;							//補完中か？
		std::vector<AnimationEventListener> m_animationEventListeners;	//アニメーションのイベントリスナー。
		Vector3 m_footstepDeltaValue = Vector3::Zero;			//footstepボーンの移動量。			//補完中かどうか。
		float m_deltaTimeOnUpdate = 0.0f;						//Update関数を実行したときのデルタタイム。
		bool m_isInited = false;					//ロード済みかどうか。
	};
	////////////CAnimationClass

}