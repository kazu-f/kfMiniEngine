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

	//アニメーションクリップの初期化データ。
	struct AnimClipInitData
	{
		std::string tkaFilePath;		//tkaファイルのパス。
		bool isLoop = false;			//ループフラグ。
	};

	/// <summary>
	/// アニメーションイベント。
	/// </summary>
	class CAnimationEvent : Noncopyable{
	public:
		/// <summary>
		/// イベント発生時間を取得。
		/// </summary>
		float GetInvokeTime()const
		{
			return m_invokeTime;
		}
		/// <summary>
		/// イベント名取得。
		/// </summary>
		const wchar_t* GetEventName()const
		{
			return m_eventName.c_str();
		}
		/// <summary>
		/// イベント発生時間を設定。
		/// </summary>
		void SetInvokeTime(float time)
		{
			m_invokeTime = time;
		}
		/// <summary>
		/// イベント名を設定。
		/// </summary>
		void SetEventName(const wchar_t* name)
		{
			m_eventName = name;
		}
		/// <summary>
		/// イベントが発生済みか判定。
		/// </summary>
		bool IsInvoked()const
		{
			return m_isInvoke;
		}
		/// <summary>
		/// イベントが発生済みのフラグを設定する。
		/// </summary>
		void SetInvokedFlag(bool flag)
		{
			m_isInvoke = flag;
		}

	private:
		bool m_isInvoke = false;	//イベント発生済み？
		float m_invokeTime = 0.0f;	//イベント発生時間。
		std::wstring m_eventName;	//イベント名。
	};
	

	class CAnimationClip:Noncopyable
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
		//ボーン毎のキーフレームのリストを取得。
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
		/// <summary>
		/// アニメーションイベントを取得。
		/// </summary>
		std::unique_ptr<CAnimationEvent[]>& GetAnimationEvent()
		{
			return m_animationEvent;
		}
		/// <summary>
		/// アニメーションイベントの数を取得。
		/// </summary>
		int GetNumAnimationEvent()const
		{
			return m_tkaFile.GetNumAnimationEvent();
		}
	private:
		using KeyframePtr = std::unique_ptr<KeyFrame>;
		TkaFile m_tkaFile;						//tkaファイル。
		std::wstring m_clipName;				//アニメーションクリップの名前。
		std::vector<KeyframePtr> m_keyframes;			//全てのキーフレーム。
		std::vector<keyFramePtrList> m_keyFramePtrListArray;		//ボーン毎のキーフレームのリストを管理するための配列。
		std::unique_ptr<CAnimationEvent[]> m_animationEvent;		//アニメーションイベント。
		int m_numAnimationEvent = 0;								//アニメーションイベントの数。
		keyFramePtrList* m_topBoneKeyFrameList = nullptr;		//ルートボーン？のキーフレームリスト。

		bool m_isLoaded = false;
		bool m_isLoop = false;			//ループフラグ。
	};
	//////////////////////CAnimationClip
	using CAnimationClipPtr = std::unique_ptr<CAnimationClip>;
}