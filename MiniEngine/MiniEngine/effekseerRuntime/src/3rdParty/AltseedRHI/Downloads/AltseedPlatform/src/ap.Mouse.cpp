#include "ap.Mouse.h"

#if defined(_PSVITA)

#elif defined(__ANDROID__)

#else
#include "PC/ap.Mouse_Impl_PC.h"
#endif

namespace ap
{
	Mouse* Mouse::Create(Window* window)
	{
#if defined(_PSVITA)
		return new Mouse();
#elif defined(__ANDROID__)
		return new Mouse();
#else
		return new Mouse_Impl_PC(window);
#endif
	}
}