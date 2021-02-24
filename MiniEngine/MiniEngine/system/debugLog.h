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
		OutputDebugString(log);
		OutputDebugString("\n");
		va_end(va);
	}
	/// <summary>
	/// 出力に警告を表示する。
	/// </summary>
	static inline void WarningLog(const char* format, ...)
	{
		static char newFormat[1024 * 10];
		strcpy_s(newFormat, "ENGINE_WARNING : ");
		strcat_s(newFormat, format);

		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		vsprintf_s(log, newFormat, va);
		OutputDebugString(log);
		OutputDebugString("\n");
		va_end(va);
	}
	/// <summary>
	/// 警告のメッセージボックス。
	/// </summary>
	static inline void WarningMessageBox(const char* file, long line, const char* format, ...)
	{
		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		char fileLineInfo[256];
		sprintf_s(fileLineInfo, "\n\n%s,%d行目", file, line);
		vsprintf_s(log, format, va);
		strcat_s(log, fileLineInfo);
		va_end(va);

		MessageBox(NULL, log, "Warning!!", MB_OK);
	}
	/// <summary>
	/// 警告のメッセージボックス。
	/// </summary>
	/// <remarks>
	/// flagが偽の時にアサートが発生する。
	/// </remarks>
	static inline void DebugAssert(bool flag,const char* format,const char* file, long line, ...)
	{
		if (!flag)
		{
			va_list va;
			va_start(va, flag);
			vprintf(format, va);
			char fileLineInfo[256];
			sprintf_s(fileLineInfo, "%s,%d行目", file, line);
			char assertMessage[256];
			vsprintf_s(assertMessage, format, va);
			strcat_s(assertMessage, fileLineInfo);
			MessageBox(NULL, assertMessage, "Erorr!!!", MB_OK);
			va_end(va);
			std::abort();
		}
	}
}

#if _DEBUG
	#define ENGINE_LOG(format,...)				DebugLog(format,__VA_ARGS__);								//デバッグ用のログを出力する。
#define ENGINE_WARNING_LOG(format,...)			WarningLog(format, __VA_ARGS__);							//警告ログを出力する。
	#define ENGINE_MESSAGE_BOX(format,...)		WarningMessageBox(__FILE__,__LINE__,format,__VA_ARGS__);	//メッセージボックスを表示。
	#define ENGINE_ASSERT(flag,format,...)		DebugAssert(flag,format,__FILE__,__LINE__,__VA_ARGS__);		//アサートする。
#else
	#define ENGINE_LOG(format,...)
	#define ENGINE_WARNING_LOG(format,...)
	#define ENGINE_MESSAGE_BOX(format,...)
	#define ENGINE_ASSERT(flag,format,...)
#endif // _DEBUG

#endif // _DEBUG_LOG_H_


