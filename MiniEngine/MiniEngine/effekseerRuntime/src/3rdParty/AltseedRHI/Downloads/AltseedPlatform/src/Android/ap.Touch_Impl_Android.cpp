#include "ap.Touch_Impl_Android.h"
#include <android/input.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "threaded_app", __VA_ARGS__))

namespace ap
{
	Touch_Impl_Android::Touch_Impl_Android(Window* window)
	{
		this->window = (Window_Impl_Android*)window;
	}

	Touch_Impl_Android::~Touch_Impl_Android()
	{

	}

	void Touch_Impl_Android::GetPosition(float& x, float& y)
	{
		auto engine = window->engine;
		LOGI("Touch GetPosition: X=%.1f Y=%.1f", engine.touchX, engine.touchY);
		x = engine.touchX;
		y = engine.touchY;
	}
}