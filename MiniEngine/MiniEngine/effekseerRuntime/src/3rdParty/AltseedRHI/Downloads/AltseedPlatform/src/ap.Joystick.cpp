#include "ap.Joystick.h"

#if defined(_PSVITA)
#include "PSVITA/ap.Joystick_Impl_PSVITA.h"
#elif defined(_PS4)
#include "PS4/ap.Joystick_Impl_PS4.h"
#elif defined(_SWITCH)
#include "SWITCH/ap.Joystick_Impl_SWITCH.h"
#elif defined(_XBOXONE)
#include "XBOXONE/ap.Joystick_Impl_XBOXONE.h"
#elif defined(__ANDROID__)

#else
#include "PC/ap.Joystick_Impl_PC.h"
#endif

namespace ap
{
	Joystick* Joystick::Create(Window* window)
	{
#if defined(_PSVITA)
		return new Joystick_Impl_PSVITA(window);
#elif defined(_PS4)
		return new Joystick_Impl_PS4(window);
#elif defined(_SWITCH)
		return new Joystick_Impl_SWITCH(window);
#elif defined(_XBOXONE)
		return new Joystick_Impl_XBOXONE(window);
#elif defined(__ANDROID__)
		return new Joystick();
#else
		return new Joystick_Impl_PC(window);
#endif
	}
}