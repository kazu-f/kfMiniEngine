#include "ap.Window_Impl_PC.h"

#include <codecvt>
#include <locale>

namespace ap
{
	// http://hasenpfote36.blogspot.jp/2016/09/stdcodecvt.html
	static constexpr std::codecvt_mode mode = std::codecvt_mode::little_endian;

	static std::string utf16_to_utf8(const std::u16string& s)
	{
#if defined(_MSC_VER)
		std::wstring_convert<std::codecvt_utf8_utf16<std::uint16_t, 0x10ffff, mode>, std::uint16_t> conv;
		auto p = reinterpret_cast<const std::uint16_t*>(s.c_str());
		return conv.to_bytes(p, p + s.length());
#else
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, mode>, char16_t> conv;
		return conv.to_bytes(s);
#endif
	}

	void GLFLW_ResizeCallback(GLFWwindow* w, int x, int y)
	{
		auto w_ = (Window_Impl_PC*)glfwGetWindowUserPointer(w);

		if (w_->Resized != nullptr)
		{
			w_->Resized(x, y);
		}
	}

	static void GLFLW_FocusCallback(GLFWwindow* window, int b)
	{
		auto w_ = (Window_Impl_PC*)glfwGetWindowUserPointer(window);

		if (w_->Focused != nullptr)
		{
			w_->Focused(b == GL_TRUE);
		}
	}

	void Window_Impl_PC::Terminate()
	{
		if (window != nullptr)
		{
			glfwDestroyWindow(window);
			window = nullptr;
			glfwTerminate();
		}
	}

	Window_Impl_PC::Window_Impl_PC()
	{

	}
	
	Window_Impl_PC::~Window_Impl_PC()
	{
		Terminate();
	}

	ErrorCode Window_Impl_PC::Initialize(const WindowInitializationParameter& parameter)
	{
		if (!glfwInit())
		{
			return ErrorCode::FailedInitializeWindowSystem;
		}

#ifdef __APPLE__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

		if (parameter.GraphicsDevice == GraphicsDeviceType::OpenGL)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		}
		else
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			isOpenGLMode = false;
		}

		glfwWindowHint(GLFW_RESIZABLE, parameter.IsResizable ? GL_TRUE : GL_FALSE);

		if (parameter.ColorSpace == ColorSpaceType::LinearSpace)
		{
			glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
		}

		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

		GLFWmonitor* monitor = nullptr;
		if (parameter.IsFullscreenMode)
		{
			monitor = glfwGetPrimaryMonitor();
		}

		window = glfwCreateWindow(parameter.WindowWidth, parameter.WindowHeight, utf16_to_utf8(parameter.Title).c_str(), monitor, nullptr);
		if (window == nullptr)
		{
			glfwTerminate();
			return ErrorCode::FailedCreateWindow;
		}

		glfwSwapInterval(1);

		// Centering
		if (!parameter.IsFullscreenMode && parameter.WindowPosition == WindowPositionType::Centering)
		{
			auto monitor = glfwGetPrimaryMonitor();
			
			if (monitor != nullptr)
			{
				auto videomode = glfwGetVideoMode(monitor);

				if (videomode != nullptr)
				{
					int32_t x = 0;
					int32_t y = 0;

					glfwGetMonitorPos(monitor, &x, &y);

					auto offset_x = (videomode->width - parameter.WindowWidth) / 2 + x;
					auto offset_y = (videomode->height - parameter.WindowHeight) / 2 + y;

					glfwSetWindowPos(window, offset_x, offset_y);
				}
			}
		}

		glfwShowWindow(window);

		glfwSwapInterval(0);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, GLFLW_ResizeCallback);
		glfwSetWindowFocusCallback(window, GLFLW_FocusCallback);

		return ErrorCode::OK;
	}

	bool Window_Impl_PC::DoEvent()
	{
		if (window == nullptr) return false;

		if (glfwWindowShouldClose(window))
		{
			Terminate();
			return false;
		}

		glfwPollEvents();
		return true;
	}

	void Window_Impl_PC::Close()
	{
		glfwSetWindowShouldClose(window, 1);
	}

	void Window_Impl_PC::SetTitle(const char16_t* title)
	{
		auto titleUTF8 = utf16_to_utf8(title);
		glfwSetWindowTitle(window, titleUTF8.c_str());
	}

	void Window_Impl_PC::SetSize(int32_t width, int32_t height)
	{
		glfwSetWindowSize(window, width, height);
	}

	void Window_Impl_PC::GetFrameBufferSize(int32_t& width, int32_t& height)
	{
		if (isOpenGLMode)
		{
			glfwGetFramebufferSize(window, &width, &height);
		}
		else
		{
			glfwGetWindowSize(window, &width, &height);
		}
	}	

	void Window_Impl_PC::Present()
	{
		if (isOpenGLMode && window != nullptr)
		{
			glfwSwapBuffers(window);
		}
	}

	void Window_Impl_PC::MakeContextCurrent()
	{
		if (isOpenGLMode && window != nullptr)
		{
			glfwMakeContextCurrent(window);
		}
	}

	void Window_Impl_PC::MakeContextNone()
	{
		if (isOpenGLMode && window != nullptr)
		{
			glfwMakeContextCurrent(nullptr);
		}
	}

	void* Window_Impl_PC::GetHandle() const
	{
#ifdef _WIN32
		return glfwGetWin32Window(window);
#else
		return nullptr;
#endif
	}

	void Window_Impl_PC::GetMonitorSize(int32_t& width, int32_t& height)
	{
		auto monitor = glfwGetPrimaryMonitor();

		if (monitor != nullptr)
		{
			
			auto videomode = glfwGetVideoMode(monitor);

			if (videomode != nullptr)
			{
				width = videomode->width;
				height = videomode->height;
			}
		}
	}
}