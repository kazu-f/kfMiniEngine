#include "ap.Mouse_Impl_PC.h"

#include "ap.Window_Impl_PC.h"

namespace ap
{

static double wheelTemp = 0;
static bool wheelCalled = false;
std::function<void(double x, double y)> wheelCallback;

static void GetWheelInternal(GLFWwindow* wHandle, double x, double y)
{
	wheelTemp = y;
	wheelCalled = true;

	if (wheelCallback != nullptr)
	{
		wheelCallback(x, y);
	}
}

Mouse_Impl_PC::Mouse_Impl_PC(Window* window)
	: window(window)
{
	auto w = (Window_Impl_PC*)window;
	auto w_ = (GLFWwindow*)window->GetNativeWindow();

	glfwSetScrollCallback(w_, GetWheelInternal);
}

Mouse_Impl_PC::~Mouse_Impl_PC()
{

}

void Mouse_Impl_PC::RefreshInputState()
{
	auto w = (Window_Impl_PC*)window;
	auto w_ = (GLFWwindow*)window->GetNativeWindow();

	double mx, my;
	glfwGetCursorPos(w_, &mx, &my);
	posX = mx;
	posY = my;

	wheel = (wheelCalled) ? wheelTemp : 0;
	wheelCalled = false;

	preHit = currentHit;

	for (int32_t i = 0; i < KEY_NUM; i++)
	{
		currentHit[i] = (bool)glfwGetMouseButton(w_, i);
	}
}

void Mouse_Impl_PC::SetPosition(float x, float y)
{
	auto w = (Window_Impl_PC*)window;
	auto w_ = (GLFWwindow*)window->GetNativeWindow();
	glfwSetCursorPos(w_, x, y);

	Mouse::SetPosition(x, y);
}

void Mouse_Impl_PC::SetWheelCallback(std::function<void(double x, double y)> func)
{
	wheelCallback = func;
}

float Mouse_Impl_PC::GetWheel() const
{
	return wheel;
}


InputState Mouse_Impl_PC::GetMouseButtonState(MouseButtons button) const
{
	int index = (int32_t)button;
	if (currentHit[index] && preHit[index]) return InputState::Hold;
	else if (!currentHit[index] && preHit[index]) return InputState::Release;
	else if (currentHit[index] && !preHit[index]) return InputState::Push;
	else return InputState::Free;
}

}