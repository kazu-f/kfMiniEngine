#pragma once

/// <summary>
/// �A�j���[�V�����B
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
		/// �������ς݂����肷��B
		/// </summary>
		/// <returns>true�ŏ������ς݁B</returns>
		bool IsInited()const
		{
			return m_isInited;
		}
		/// <summary>
		/// �A�j���[�V�����̏����������B
		/// </summary>
		/// <param name="skinModel">�A�j���[�V���������郂�f���B</param>
		/// <param name="animClipList">�A�j���[�V�����N���b�v�̃��X�g�B</param>
		/// <param name="animClipNum">�A�j���[�V�����N���b�v�̐��B</param>
		void Init(Model& skinModel, CAnimationClip animClipList[], int numAnimClip);
		/// <summary>
		/// �A�j���[�V�����̍Đ��B
		/// </summary>
		/// <param name="clipNo">�A�j���[�V�����N���b�v�ԍ��B</param>
		/// <param name="interpolateTime">�⊮���ԁB</param>
		void Play(int clipNo, float interpolateTime = 0.0f)
		{
			if (clipNo < m_animationClips.size()) {
				PlayCommon(m_animationClips[clipNo], interpolateTime);
			}
		}
		/// <summary>
		/// �A�j���[�V�����N���b�v�̃��[�v�t���O��ݒ肷��B
		/// </summary>
		/// <param name="clipName">�A�j���[�V�����N���b�v�̖��O</param>
		/// <param name="flag">�t���O</param>
		void SetAnimationClipLoopFlag(const wchar_t* clipName, bool flag)
		{
			auto it = std::find_if(
				m_animationClips.begin(),
				m_animationClips.end(),
				[clipName](auto & clip) { return clip->GetName() == clipName; }
			);
			if (it == m_animationClips.end())
			{
				//������Ȃ������B
				return;
			}
			(*it)->SetLoopFlag(flag);
		}
		/// <summary>
		/// �A�j���[�V�����̍Đ������H
		/// </summary>
		/// <returns></returns>
		bool IsPlaying() const
		{
			
		}
		/// <summary>
		/// �A�j���[�V������i�߂�B
		/// </summary>
		/// <param name="deltaTime">�A�j���[�V������i�߂鎞��(s)</param>
		void Update(float deltaTime);
	private:
		/// <summary>
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		/// <param name="nectClip"></param>
		/// <param name="interpolateTime"></param>
		void PlayCommon(CAnimationClip* nectClip, float interpolateTime)
		{

		}
		/// <summary>
		/// ���[�J���|�[�Y�̍X�V�B
		/// </summary>
		/// <param name="deltaTime">�A�j���[�V������i�߂鎞��(s)</param>
		void UpdateLocalPose(float deltaTime);
		/// <summary>
		/// �O���[�o���|�[�Y�̍X�V�B
		/// </summary>
		void UpdateGlobalPose();

	private:
		static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;	//�A�j���[�V�����R���g���[���̐��B
		std::vector<CAnimationClip*>	m_animationClips;		//�A�j���[�V�����N���b�v�̔z��B
		Skeleton* m_skelton = nullptr;							//�A�j���[�V������K�p����X�P���g���B
		CAnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];		//�A�j���[�V�����v���C�R���g���[���B
		int m_numAnimationPlayController = 0;					//���ݎg�p���̃A�j���[�V�����Đ��R���g���[���̐��B
		int m_startAnimationPlayController = 0;					//�A�j���[�V�����R���g���[���̊J�n�C���f�b�N�X�B
		float m_interpolateTime = 0.0f;							//�⊮���ԁB
		float m_interpolateTimeEnd = 0.0f;						//�⊮�I�����ԁB�H
		float m_deltaTimeOnUpdate = 0.0f;						//Update�֐������s�����Ƃ��̃f���^�^�C���B
		bool m_isInterpolate = false;				//�⊮�����ǂ����B
		bool m_isInited = false;					//���[�h�ς݂��ǂ����B
	};
	////////////CAnimationClass

}