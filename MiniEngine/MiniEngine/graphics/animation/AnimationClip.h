#pragma once

#include "tkFile/TkaFile.h"

/// <summary>
/// �A�j���[�V�����N���b�v�B
/// </summary>

namespace Engine {

	/// <summary>
	/// �L�[�t���[���B
	/// </summary>
	struct KeyFrame {
		std::uint32_t boneIndex;	//�{�[���C���f�b�N�X�B
		float time;					//���ԁB
		Matrix transform;			//�g�����X�t�H�[���B
	};
	///// <summary>
	///// �ǂݍ��݂Ɏg���L�[�t���[���\���́B
	///// </summary>
	//struct KeyframeRow
	//{
	//	std::uint32_t boneIndex;	//�{�[���C���f�b�N�X�B
	//	float time;					//���ԁB
	//	Vector3 transform[4];	//�g�����X�t�H�[���B
	//};

	/// <summary>
	/// �A�j���[�V�����C�x���g�B
	/// </summary>
	

	class CAnimationClip
	{
	public:
		typedef std::vector<KeyFrame*>		keyFramePtrList;

		CAnimationClip() {};
		~CAnimationClip();
		/// <summary>
		/// �A�j���[�V�����N���b�v�𓯊����[�h�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void Load(const char* filePath);
		/// <summary>
		/// �A�j���[�V�����N���b�v�̔񓯊����[�h�B
		/// IsLoadead�֐����g�p���ē��������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void LoadAsync(const char* filePath)
		{
			//�悭�킩��񂩂��񂵁B
		}
		/// <summary>
		/// �ǂݍ��݂��I����Ă��邩����B
		/// </summary>
		/// <returns></returns>
		bool IsLoaded()const
		{
			return m_isLoaded;
		}
		/// <summary>
		/// �L�[�t���[���ƃA�j���[�V�����C�x���g���\�z����B
		/// </summary>
		/// <remarks>
		/// ���[�h���I������Ƃ��ɌĂяo���B
		/// </remarks>
		void BuildKeyFramesAndAnimationEvents();
		//���[�v���邩�H
		bool IsLoop()const
		{
			return m_isLoop;
		}
		//���[�v�t���O��ݒ肷��B
		void SetLoopFlag(bool flag)
		{
			m_isLoop = flag;
		}

		const std::vector<keyFramePtrList>& GetKeyFramePtrListArray()const
		{
			return m_keyFramePtrListArray;
		}
		const keyFramePtrList& GetTopBoneKeyFrameList()const
		{
			return *m_topBoneKeyFrameList;
		}
		/// <summary>
		/// �N���b�v�����擾�B
		/// </summary>
		const wchar_t* GetName()const
		{
			return m_clipName.c_str();
		}		

	private:
		using KeyframePtr = std::unique_ptr<KeyFrame>;
		TkaFile m_tkaFile;						//tka�t�@�C���B
		std::wstring m_clipName;				//�A�j���[�V�����N���b�v�̖��O�B
		std::vector<KeyframePtr> m_keyframes;			//�S�ẴL�[�t���[���B
		std::vector<keyFramePtrList> m_keyFramePtrListArray;		//�{�[�����̃L�[�t���[���̃��X�g���Ǘ����邽�߂̔z��B

		keyFramePtrList* m_topBoneKeyFrameList = nullptr;		//���[�g�{�[���H�̃L�[�t���[�����X�g�B

		bool m_isLoaded = false;
		bool m_isLoop = false;			//���[�v�t���O�B
	};
	//////////////////////CAnimationClip
	using CAnimationClipPtr = std::unique_ptr<CAnimationClip>;
}