#pragma once

#include "ap.AndroidNativeActivityGlue.h"
#include "ap.Window_Impl_Android.h"
#include "../ap.Touch.h"

namespace ap
{
	class Touch_Impl_Android
		: public Touch
	{
	private:
		Window_Impl_Android* window;
	public:
		Touch_Impl_Android(Window* window);
		~Touch_Impl_Android();

		void GetPosition(float& x, float& y) override;

	};
}