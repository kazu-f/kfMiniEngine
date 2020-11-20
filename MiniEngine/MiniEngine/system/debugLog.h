#pragma once

#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_
namespace Engine {
	/// <summary>
	/// 出力にログを表示する。
	/// </summary>
	static inline void DebugLog(const char* format, ...)
	{
		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		vsprintf_s(log, format, va);
		OutputDebugStringA(log);
		OutputDebugStringA("\n");
		va_end(va);
	}
	/// <summary>
	/// 警告のメッセージボックス。
	/// </summary>
	inline static void WarningMessageBox(const char* text)
	{
		MessageBoxA(NULL, text, "Warning!!!", MB_OK);
	}
	/// <summary>
	/// 警告のメッセージボックス。
	/// </summary>
	inline static void DebugAssert(const char* text)
	{
		MessageBoxA(NULL, text, "Warning!!!", MB_OK);
		std::abort();
	}
}

#if _DEBUG
	#define DEBUG_LOG(text)				DebugLog(text);				//デバッグ用のログを出力する。
	#define WARNING_MESSAGE_BOX(text)	WarningMessageBox(text);	//メッセージボックスを表示。
	#define ASSERT(text)				DebugAssert(text);			//アサートする。
#else
	#define DEBUG_LOG(text)
	#define WARNING_MESSAGE_BOX(text)
	#define ASSERT(text)
#endif // _DEBUG

#endif // _DEBUG_LOG_H_


