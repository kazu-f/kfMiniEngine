#include "ap.Keyboard.h"

#if defined(_PSVITA)

#elif defined(__ANDROID__)

#else
#include "PC/ap.Keyboard_Impl_PC.h"
#endif

namespace ap
{
	Keyboard* Keyboard::Create(Window* window)
	{
#if defined(_PSVITA)
		return new Keyboard();
#elif defined(__ANDROID__)
		return new Keyboard();
#else
		return new Keyboard_Impl_PC(window);
#endif
	}
}