#pragma once

#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_
namespace Engine {
	/// <summary>
	/// 警告のメッセージボックス。
	/// </summary>
	inline static void WarningMessageBox(const char* text)
	{
		MessageBoxA(NULL, text, "Warning!!!", MB_OK);
	}
	/// <summary>
	/// 
	/// </summary>
	//inline statc void 
}

#if _DEBUG
	#define WARNING_MESSAGE_BOX(text)	WarningMessageBox(text);
#else
	#define WARNING_MESSAGE_BOX(text)
	
#endif // _DEBUG

#endif // _DEBUG_LOG_H_


