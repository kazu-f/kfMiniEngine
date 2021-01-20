
#pragma once 

#include "../ap.Window.h"

namespace ap
{

	class Window_Impl_PC
		: public Window
	{
	private:
		GLFWwindow*		window = nullptr;
		bool			isOpenGLMode = true;
		
		void Terminate();

	public:
		Window_Impl_PC();
		virtual ~Window_Impl_PC();

		ErrorCode Initialize(const WindowInitializationParameter& parameter) override;

		bool DoEvent() override;

		void Close() override;

		void SetTitle(const char16_t* title) override;

		void SetSize(int32_t width, int32_t height) override;

		void GetFrameBufferSize(int32_t& width, int32_t& height) override;

		void* GetHandle() const override;

		void GetMonitorSize(int32_t& width, int32_t& height) override;

		void Present() override;

		void MakeContextCurrent() override;

		void MakeContextNone() override;

		void* GetNativeWindow() const override { return window; }
	};

}