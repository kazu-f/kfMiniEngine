#pragma once

#include "ap.Base.h"

namespace ap
{

class Window
{
public:
	Window() {}
	virtual ~Window() {}

	virtual ErrorCode Initialize(const WindowInitializationParameter& parameter) = 0;

	virtual bool DoEvent() = 0;

	virtual void Close() {}

	static Window* Create();
};

}