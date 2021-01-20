#include "ap.Window.h"

#if defined(_PSVITA)
#include "PSVITA/ap.Window_Impl_PSVITA.h"
#elif defined(_PS4)
#include "PS4/ap.Window_Impl_PS4.h"
#elif defined(_SWITCH)
#include "SWITCH/ap.Window_Impl_SWITCH.h"
#elif defined(_XBOXONE)
#include "XBOXONE/ap.Window_Impl_XBOXONE.h"
#elif defined(__ANDROID__)
#include "Android/ap.Window_Impl_Android.h"
#else
#include "PC/ap.Window_Impl_PC.h"
#endif

namespace ap
{
	Window* Window::Create()
	{
#if defined(_PSVITA)
		return new Window_Impl_PSVITA();
#elif defined(_PS4)
		return new Window_Impl_PS4(window);
#elif defined(_SWITCH)
		return new Window_Impl_SWITCH(window);
#elif defined(_XBOXONE)
		return new Window_Impl_XBOXONE(window);
#elif defined(__ANDROID__)
		return new Window_Impl_Android();
#else
		return new Window_Impl_PC();
#endif
	}
}