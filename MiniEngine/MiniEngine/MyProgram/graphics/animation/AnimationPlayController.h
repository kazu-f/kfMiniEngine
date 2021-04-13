#pragma once

/// <summary>
/// アニメーションプレイコントローラー。
/// </summary>

namespace Engine {
	class Skeleton;
	class CAnimationClip;
	class CAnimation;

	/// <summary>
	/// アニメーションの再生コントローラ。
	/// </summary>
	/// <remarks>
	/// 一つのアニメーションクリップに対してアニメーションを進めて、ローカルポーズを計算する。
	/// </remarks>

	class CAnimationPlayController : Noncopyable
	{
	public:
		//コンストラクタ。
		CAnimationPlayController()
		{
		}
		//デストラクタ。
		~CAnimationPlayController()
		{
		}
		/// <summary>
		/// 初期化処理。
		/// </summary>
		/// <param name="skeleton">スケルトンデータのアドレス</param>
		/// <param name="footStepBoneNo">footstepボーンの番号</param>
		void Init(Skeleton* skeleton, int footStepBoneNo);
		/// <summary>
		/// アニメーションクリップの変更。
		/// </summary>
		/// <param name="clip">変更先のクリップ</param>
		void ChangeAnimationClip(CAnimationClip* clip)
		{
			m_animationClip = clip;
			m_currentKeyFrameNo = 0;
			m_time = 0.0f;
			m_isPlaying = true;
			m_footstepPos = Vector3::Zero;
			m_footstepDeltaValue = Vector3::Zero;
			//アニメーションイベントの発生フラグを全てfalseにする。
			auto& animEventArray = m_animationClip->GetAnimationEvent();
			for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
				animEventArray[i].SetInvokedFlag(false);
			}
		}
		/// <summary>
		/// 補完時間の設定かな。
		/// </summary>
		void SetInterpolateTime(float interpolateTime)
		{
			if (interpolateTime < 0.0f)
			{
				MessageBoxA(NULL, "アニメーションの補完時間がマイナスの値！", "Warning!!", MB_OK);
			}
			m_interpolateEndTime = interpolateTime;
			m_interpolateTime = 0.0f;
		}
		/// <summary>
		/// 補完率を取得する。
		/// </summary>
		float GetInterpolateRate()const
		{
			if (m_interpolateEndTime <= 0.0f)
			{
				return 1.0f;
			}
			return min(1.0f, m_interpolateTime / m_interpolateEndTime);
		}
		/// <summary>
		/// アニメーションを進める。
		/// </summary>
		/// <param name="deltaTime">アニメーションを進める時間。</param>
		/// <param name="animation">アニメーションクラスのアドレス。</param>
		void Update(float deltaTime, CAnimation* animation);
		/// <summary>
		/// ローカルボーン行列を取得。
		/// </summary>
		const std::vector<Matrix>& GetBoneLocalMatrix()const
		{
			return m_boneMatrix;
		}
		/// <summary>
		/// アニメーションクリップの取得。
		/// </summary>
		CAnimationClip* GetAnimClip()const
		{
			return m_animationClip;
		}
		/// <summary>
		/// アニメーションを再生中？
		/// </summary>
		bool IsPlaying()const
		{
			return m_isPlaying;
		}
		/// <summary>
		/// Update関数を呼び出した時の、footstepボーン移動量を取得。
		/// </summary>
		Vector3 GetFootStepDeltaValueOnUpdate()const
		{
			return m_footstepDeltaValue;
		}
	private:	//////////クラス内部関数。
		/// <summary>
		/// アニメーションイベントを起動する。
		/// </summary>
		void InvokeAnimationEvent(CAnimation* animation);
		/// <summary>
		/// ループ再生を開始するときの処理。
		/// </summary>
		void StartLoop();
		/// <summary>
		/// ルートのボーン空間でのボーン行列を計算する。
		/// </summary>
		/// <param name="bone">計算する骨</param>
		/// <param name="parentMatrix">親の行列</param>
		void CalcBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix);
		/// <summary>
		/// ボーン行列をアニメーションクリップからサンプリングする。
		/// </summary>
		void SamplingBoneMatrixFromAnimationClip();
		/// <summary>
		/// ルートボーン空間での行列を計算する。引数なしVer
		/// </summary>
		void CalcBoneMatrixInRootBoneSpace();
		/// <summary>
		/// footstepボーンの変化量をサンプリングする。
		/// </summary>
		void SamplingDataValueFootstepBone();
		/// <summary>
		/// footstepボーンの座標を全体の骨から減算する。
		/// </summary>
		void SubtractFootstepBonePosFromAllBone();
		/// <summary>
		/// キーフレームを進める。
		/// </summary>
		void ProgressKeyframeNo(float deltaTime);

	private:
		CAnimationClip* m_animationClip = nullptr;		//アニメーションクリップ。
		int m_currentKeyFrameNoLastFrame = 0;			//一フレーム前のキーフレーム番号。
		int m_currentKeyFrameNo = 0;					//現在再生中のキーフレーム番号。
		float m_time = 0.0f;								//なんかの時間？なんだろ？
		std::vector<Matrix>	m_boneMatrix;				//このコントローラで再生中のアニメーションのボーン行列。
		float m_interpolateTime = 0.0f;					//補完時間。
		float m_interpolateEndTime = 0.0f;				//補完終了時間。
		bool m_isPlaying = false;						//再生中か？
		Skeleton* m_skeleton = nullptr;					//スケルトンデータ。
		Vector3 m_footstepDeltaValue = Vector3::Zero;	//フットステップの移動ベクトル。
		Vector3 m_footstepPos = Vector3::Zero;			//フットステップボーンの座標。
		int m_footstepBoneNo = -1;						//フットステップのボーン番号。
	};
	/////////AnimationPlayController

}