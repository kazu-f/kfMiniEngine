
#pragma once 

#include "ap.AndroidNativeActivityGlue.h"
#include "ap.Window.h"

namespace ap
{

	class Window_Impl_Android
		: public Window
	{
	private:
		
	public:
		Window_Impl_Android();
		virtual ~Window_Impl_Android();


		ErrorCode Initialize(const WindowInitializationParameter& parameter) override;
		void Update();

		bool DoEvent() override;

		//struct android_app* state;
		struct engine engine;
	};

}