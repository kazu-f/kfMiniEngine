#pragma once

#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_
namespace Engine {
	/// <summary>
	/// �o�͂Ƀ��O��\������B
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
	/// �x���̃��b�Z�[�W�{�b�N�X�B
	/// </summary>
	inline static void WarningMessageBox(const char* text)
	{
		MessageBoxA(NULL, text, "Warning!!!", MB_OK);
	}
	/// <summary>
	/// �x���̃��b�Z�[�W�{�b�N�X�B
	/// </summary>
	inline static void DebugAssert(const char* text)
	{
		MessageBoxA(NULL, text, "Warning!!!", MB_OK);
		std::abort();
	}
}

#if _DEBUG
	#define DEBUG_LOG(text)				DebugLog(text);				//�f�o�b�O�p�̃��O���o�͂���B
	#define WARNING_MESSAGE_BOX(text)	WarningMessageBox(text);	//���b�Z�[�W�{�b�N�X��\���B
	#define ASSERT(text)				DebugAssert(text);			//�A�T�[�g����B
#else
	#define DEBUG_LOG(text)
	#define WARNING_MESSAGE_BOX(text)
	#define ASSERT(text)
#endif // _DEBUG

#endif // _DEBUG_LOG_H_


