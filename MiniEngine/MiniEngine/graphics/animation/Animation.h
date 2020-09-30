#pragma once

/// <summary>
/// アニメーション。
/// </summary>

#include "AnimationClip.h"
#include "AnimationPlayController.h"

namespace Engine {
	class Skeleton;

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
		void Init(Model& skinModel, CAnimationClip animClipList[], int numAnimClip);
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
			
		}
		/// <summary>
		/// アニメーションを進める。
		/// </summary>
		/// <param name="deltaTime">アニメーションを進める時間(s)</param>
		void Update(float deltaTime);
	private:
		/// <summary>
		/// アニメーションの再生
		/// </summary>
		/// <param name="nectClip"></param>
		/// <param name="interpolateTime"></param>
		void PlayCommon(CAnimationClip* nectClip, float interpolateTime)
		{

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
		static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;	//アニメーションコントローラの数。
		std::vector<CAnimationClip*>	m_animationClips;		//アニメーションクリップの配列。
		Skeleton* m_skelton = nullptr;							//アニメーションを適用するスケルトン。
		CAnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];		//アニメーションプレイコントローラ。
		int m_numAnimationPlayController = 0;					//現在使用中のアニメーション再生コントローラの数。
		int m_startAnimationPlayController = 0;					//アニメーションコントローラの開始インデックス。
		float m_interpolateTime = 0.0f;							//補完時間。
		float m_interpolateTimeEnd = 0.0f;						//補完終了時間。？
		float m_deltaTimeOnUpdate = 0.0f;						//Update関数を実行したときのデルタタイム。
		bool m_isInterpolate = false;				//補完中かどうか。
		bool m_isInited = false;					//ロード済みかどうか。
	};
	////////////CAnimationClass

}