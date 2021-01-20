#pragma once

#include <string>

#include <stdio.h>
#include <stdint.h>
#include <array>
#include <functional>

#if defined(_PSVITA)

#elif defined(__ANDROID__)

#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>

#else

// GLFW
#if _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <X11/extensions/Xrandr.h>
#endif

#include <GLFW/glfw3.h>

#if _WIN32
#include <GLFW/glfw3native.h>
#endif

#endif

namespace ap
{
	class Window;
	class Keyboard;

	enum class ErrorCode : int32_t
	{
		OK,
		FailedInitializeWindowSystem,
		FailedCreateWindow,
		Unknown,
	};


	enum class InputState : int32_t
	{
		Push,
		Release,
		Hold,
		Free,
	};

	/**
	@brief マウスのボタン一覧を示す列挙型
	*/
	enum class MouseButtons : int32_t
	{
		/**
		@brief 左ボタン
		*/
		ButtonLeft = 0,

		/**
		@brief 右ボタン
		*/
		ButtonRight = 1,

		/**
		@brief 中央ボタン
		*/
		ButtonMiddle = 2,

		/**
		@brief サブボタン1
		*/
		SubButton1 = 3,

		/**
		@brief サブボタン2
		*/
		SubButton2 = 4,

		/**
		@brief サブボタン3
		*/
		SubButton3 = 5,

		/**
		@brief サブボタン4
		*/
		SubButton4 = 6,

		/**
		@brief サブボタン5
		*/
		SubButton5 = 7,
	};

	enum class Keys : int32_t
	{
		Unknown,
		Space,			///< スペース
		Apostrophe,		///< ： (US配列では ` )
		Comma,			///< ，
		Minus,			///< －
		Period,			///< ．
		Slash,			///< ／
		Num0,			///< 0
		Num1,			///< 1
		Num2,			///< 2
		Num3,			///< 3
		Num4,			///< 4
		Num5,			///< 5
		Num6,			///< 6
		Num7,			///< 7
		Num8,			///< 8
		Num9,			///< 9
		Semicolon,		///< ；
		Equal,			///< ＾ (US配列では ＝ )
		A,				///< A
		B,				///< B
		C,				///< C
		D,				///< D
		E,			///< E
		F,				///< F
		G,				///< G
		H,				///< H
		I,				///< I
		J,				///< J
		K,				///< K
		L,				///< L
		M,				///< M
		N,				///< N
		O,				///< O
		P,				///< P
		Q,				///< Q
		R,				///< R
		S,				///< S
		T,				///< T
		U,				///< U
		V,				///< V
		W,				///< W
		X,				///< X
		Y,				///< Y
		Z,				///< Z
		LeftBracket,	///< @ (US配列では ［ )
		Backslash,		///< ] (US配列では ＼ )
		RightBracket,	///< [ (US配列では ］ )
		GraveAccent,	///< 半角/全角
		World1,
		World2,
		Escape,			///< Esc
		Enter,			///< Enter
		Tab,			///< Tab 
		Backspace,		///< BackSpace
		Insert,			///< Insert 
		Delete,			///< Delete 
		Right,			///< →
		Left,			///< ←
		Down,			///< ↓
		Up,				///< ↑
		PageUp,			///< PageUp
		PageDown,		///< PageDown
		Home,			///< Home 
		End,			///< End
		CapsLock,		///< CapsLock
		ScrollLock,		///< ScrollLock
		NumLock,		///< NumLock
		PrintScreen,	///< PrintScreen
		Pause,			///< Pause
		F1,				///< F1
		F2,				///< F2
		F3,				///< F3
		F4,				///< F4
		F5,				///< F5
		F6,				///< F6
		F7,				///< F7
		F8,				///< F8
		F9,				///< F9
		F10,			///< F10
		F11,			///< F11
		F12,			///< F12
		F13,			///< F13
		F14,			///< F14
		F15,			///< F15
		F16,			///< F16
		F17,			///< F17
		F18,			///< F18
		F19,			///< F19
		F20,			///< F20
		F21,			///< F21
		F22,			///< F22
		F23,			///< F23
		F24,			///< F24
		F25,			///< F25
		Keypad0,		///< テンキーの0
		Keypad1,		///< テンキーの1
		Keypad2,		///< テンキーの2
		Keypad3,		///< テンキーの3
		Keypad4,		///< テンキーの4
		Keypad5,		///< テンキーの5
		Keypad6,		///< テンキーの6
		Keypad7,		///< テンキーの7
		Keypad8,		///< テンキーの8
		Keypad9,		///< テンキーの9
		KeypadDecimal,	///< テンキーの．
		KeypadDivide,	///< テンキーの／
		KeypadMultiply,	///< テンキーの＊
		KeypadSubstract,///< テンキーの－
		KeypadAdd,		///< テンキーの＋
		KeypadEnter,	///< テンキーのEnter
		KeypadEqual,	///< テンキーの＝
		LeftShift,		///< 左Shift
		LeftControl,	///< 左Ctrl
		LeftAlt,		///< 左Alt
		LeftWin,		///< 左Win
		RightShift,		///< 右Shift
		RightControl,	///< 右Ctrl
		RightAlt,		///< 右Alt
		RightWin,		///< 右Win
		Menu,			///< コンテキストメニュー
		Last,
		MAX
	};

	enum class JoystickType : int32_t
	{
		Other,
		PS4,
		XBOX360,
	};

	enum class JoystickButtonType : int32_t
	{
		Start,
		Select,
		
		LeftUp,
		LeftDown,
		LeftLeft,
		LeftRight,

		RightUp,	///< PSx Triangle
		RightRight,	///< PSx Circle
		RightLeft,	///< PSx Square
		RightDown,	///< PSx Cross

		L1,
		R1,
		L2,
		R2,
		L3,
		R3,

		LeftStart,	///< XBOX360 Start, PS4 Options
		RightStart,	///< XBOX360 Select, PS4 TouchPad

		Max,
	};

	enum class JoystickAxisType : int32_t
	{
		LeftH,
		LeftV,
		RightH,
		RightV,
		L2,
		R2,

		Max,
	};

	enum class GraphicsDeviceType : int32_t
	{
		Default = 0,		///< 実行環境で最も安定している描画方法(初期化時に使用)
		DirectX11 = 1,		///< DirectX11
		OpenGL = 2,			///< OpenGL
	};

	enum class ColorSpaceType : int32_t
	{
		GammaSpace = 0,	///< ガンマスペース
		LinearSpace = 1, ///< リニアスペース
	};

	enum class WindowPositionType : int32_t
	{
		Default = 0,		///< OSによる自動選択
		Centering = 1,		///< プライマリモニタの中央
	};


	struct WindowInitializationParameter
	{
		std::u16string	Title;
		int32_t	WindowWidth = 0;
		int32_t WindowHeight = 0;
		bool	IsFullscreenMode = false;
		bool	IsResizable = false;
		GraphicsDeviceType	GraphicsDevice = GraphicsDeviceType::Default;
		WindowPositionType	WindowPosition = WindowPositionType::Centering;
		ColorSpaceType		ColorSpace = ColorSpaceType::GammaSpace;

		std::array<void*, 3> UserData;
	};
}


namespace ap
{

class Joystick
{
private:
	std::u16string emptyStr;

public:
	Joystick() {}
	virtual ~Joystick() {}

	virtual void RefreshInputState() {}

	virtual void RefreshConnectedState() {}

	virtual bool IsPresent(int32_t joystickIndex) { return false; }

	virtual const char16_t* GetName(int32_t joystickIndex) const { return emptyStr.c_str(); }

	virtual InputState GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const { return InputState::Free; }

	virtual InputState GetButtonState(int32_t joystickIndex, JoystickButtonType type) const { return InputState::Free; }

	virtual float GetAxisState(int32_t joystickIndex, int32_t axisIndex) const { return 0; }

	virtual float GetAxisState(int32_t joystickIndex, JoystickAxisType type) const { return 0; }

	virtual JoystickType GetJoystickType(int32_t joystickIndex) const { return JoystickType::Other; }

	virtual int32_t GetButtonCount(int32_t joystickIndex) const { return 0; }

	virtual int32_t GetAxisCount(int32_t joystickIndex) const { return 0; }

	static Joystick* Create(Window* window);
};

}


namespace ap
{

class Keyboard
{
public:
	Keyboard() {}
	virtual ~Keyboard() {}

	virtual void RefreshInputState() {}

	virtual InputState GetKeyState(Keys key) const { return InputState::Free; }

	static Keyboard* Create(Window* window);
};

}


namespace ap
{

class Mouse
{
protected:
	float	posX = 0;
	float	posY = 0;

public:
	Mouse() {}
	virtual ~Mouse() {}

	virtual void RefreshInputState() {}

	virtual void GetPosition(float& x, float& y) const
	{
		x = posX;
		y = posY;
	}

	virtual void SetPosition(float x, float y)
	{
		posX = x;
		posY = y;
	}

	virtual void SetWheelCallback(std::function<void(double x, double y)> func) {}

	virtual float GetWheel() const { return 0; }

	virtual InputState GetMouseButtonState(MouseButtons button) const { return InputState::Free; }

	static Mouse* Create(Window* window);
};

}



namespace ap
{

	class Touch
	{
	public:
		Touch() {}
		virtual ~Touch() {}

		virtual void GetPosition(float& x, float& y) {}
		
		static Touch* Create(Window* window);
	};

}


namespace ap
{

class Window
{
public:

	std::function<void(int,int)>	Resized;

	std::function<void(bool)>	Focused;

	Window() {}
	virtual ~Window() {}

	virtual ErrorCode Initialize(const WindowInitializationParameter& parameter) = 0;

	virtual bool DoEvent() = 0;

	virtual void Close() {}

	virtual void SetTitle(const char16_t* title) {}

	virtual void SetSize(int32_t width, int32_t height) {}

	/**
	@brief	Get the size of frame buffer.
	@note
	This function is for OpenGL
	*/
	virtual void GetFrameBufferSize(int32_t& width, int32_t& height) {}

	/**
	@brief	Get native window pointer.
	@return	native window pointer
	@note
	PC - GLFWwindow*
	*/
	virtual void* GetNativeWindow() const { return nullptr; }

	/**
		@brief	Get native window handle.
		@return	native window handle
		@note
		Windows - HWND
	*/
	virtual void* GetHandle() const { return nullptr; }

	/**
		@brief	Get primiary monitor size.
	*/
	virtual void GetMonitorSize(int32_t& width, int32_t& height) {}

	/**
		@brief	Show back buffer.
		@note
		This function is for OpenGL
	*/
	virtual void Present() {}

	/**
	@brief	Make context current.
	@note
	This function is for OpenGL
	*/
	virtual void MakeContextCurrent() {}

	/**
	@brief	Make context none.
	@note
	This function is for OpenGL
	*/
	virtual void MakeContextNone() {}

	static Window* Create();
};

}
