#pragma once

#include "ap.Base.h"

namespace ap
{

class Window
{
public:

	std::function<void(int,int)>	Resized;

	std::function<void(bool)>	Focused;

	Window() {}
	virtual ~Window() {}

	virtual ErrorCode Initialize(const WindowInitializationParameter& parameter) = 0;

	virtual bool DoEvent() = 0;

	virtual void Close() {}

	virtual void SetTitle(const char16_t* title) {}

	virtual void SetSize(int32_t width, int32_t height) {}

	/**
	@brief	Get the size of frame buffer.
	@note
	This function is for OpenGL
	*/
	virtual void GetFrameBufferSize(int32_t& width, int32_t& height) {}

	/**
	@brief	Get native window pointer.
	@return	native window pointer
	@note
	PC - GLFWwindow*
	*/
	virtual void* GetNativeWindow() const { return nullptr; }

	/**
		@brief	Get native window handle.
		@return	native window handle
		@note
		Windows - HWND
	*/
	virtual void* GetHandle() const { return nullptr; }

	/**
		@brief	Get primiary monitor size.
	*/
	virtual void GetMonitorSize(int32_t& width, int32_t& height) {}

	/**
		@brief	Show back buffer.
		@note
		This function is for OpenGL
	*/
	virtual void Present() {}

	/**
	@brief	Make context current.
	@note
	This function is for OpenGL
	*/
	virtual void MakeContextCurrent() {}

	/**
	@brief	Make context none.
	@note
	This function is for OpenGL
	*/
	virtual void MakeContextNone() {}

	static Window* Create();
};

}