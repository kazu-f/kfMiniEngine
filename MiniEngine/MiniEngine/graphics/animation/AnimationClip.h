#pragma once

#include "tkFile/TkaFile.h"

/// <summary>
/// アニメーションクリップ。
/// </summary>

namespace Engine {

	/// <summary>
	/// キーフレーム。
	/// </summary>
	struct KeyFrame {
		std::uint32_t boneIndex;	//ボーンインデックス。
		float time;					//時間。
		Matrix transform;			//トランスフォーム。
	};
	///// <summary>
	///// 読み込みに使うキーフレーム構造体。
	///// </summary>
	//struct KeyframeRow
	//{
	//	std::uint32_t boneIndex;	//ボーンインデックス。
	//	float time;					//時間。
	//	Vector3 transform[4];	//トランスフォーム。
	//};

	/// <summary>
	/// アニメーションイベント。
	/// </summary>
	

	class CAnimationClip
	{
	public:
		typedef std::vector<KeyFrame*>		keyFramePtrList;

		CAnimationClip() {};
		~CAnimationClip();
		/// <summary>
		/// アニメーションクリップを同期ロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void Load(const char* filePath);
		/// <summary>
		/// アニメーションクリップの非同期ロード。
		/// IsLoadead関数を使用して同期を取る。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void LoadAsync(const char* filePath)
		{
			//よくわからんから後回し。
		}
		/// <summary>
		/// 読み込みが終わっているか判定。
		/// </summary>
		/// <returns></returns>
		bool IsLoaded()const
		{
			return m_isLoaded;
		}
		/// <summary>
		/// キーフレームとアニメーションイベントを構築する。
		/// </summary>
		/// <remarks>
		/// ロードが終わったときに呼び出す。
		/// </remarks>
		void BuildKeyFramesAndAnimationEvents();
		//ループするか？
		bool IsLoop()const
		{
			return m_isLoop;
		}
		//ループフラグを設定する。
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
		/// クリップ名を取得。
		/// </summary>
		const wchar_t* GetName()const
		{
			return m_clipName.c_str();
		}		

	private:
		using KeyframePtr = std::unique_ptr<KeyFrame>;
		TkaFile m_tkaFile;						//tkaファイル。
		std::wstring m_clipName;				//アニメーションクリップの名前。
		std::vector<KeyframePtr> m_keyframes;			//全てのキーフレーム。
		std::vector<keyFramePtrList> m_keyFramePtrListArray;		//ボーン事のキーフレームのリストを管理するための配列。

		keyFramePtrList* m_topBoneKeyFrameList = nullptr;		//ルートボーン？のキーフレームリスト。

		bool m_isLoaded = false;
		bool m_isLoop = false;			//ループフラグ。
	};
	//////////////////////CAnimationClip
	using CAnimationClipPtr = std::unique_ptr<CAnimationClip>;
}