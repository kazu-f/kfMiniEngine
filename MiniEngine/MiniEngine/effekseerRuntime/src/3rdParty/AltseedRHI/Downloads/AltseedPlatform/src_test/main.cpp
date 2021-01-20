
#include <stdio.h>
#include <cmath>

#include <AltseedPlatform.h>

#if defined(_PSVITA)

#elif defined(_PS4)

#elif defined(_SWITCH)

#elif defined(_XBOXONE)

#else

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"x64/Debug/glfw3.lib")
#else
#pragma comment(lib,"x64/Release/glfw3.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"x86/Debug/glfw3.lib")
#else
#pragma comment(lib,"x86/Release/glfw3.lib")
#endif

#endif

#pragma comment(lib,"opengl32.lib")

#endif

int main()
{
	auto window = ap::Window::Create();
	
	ap::WindowInitializationParameter param;
	param.WindowWidth = 640;
	param.WindowHeight = 480;

	window->Initialize(param);

	auto joystick = ap::Joystick::Create(window);

	while (window->DoEvent())
	{
		joystick->RefreshInputState();

		if (joystick->IsPresent(0))
		{
			if (joystick->GetJoystickType(0) != ap::JoystickType::Other)
			{
				if (joystick->GetButtonState(0, ap::JoystickButtonType::RightRight) == ap::InputState::Push) printf("Push : Circle\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::RightDown) == ap::InputState::Push) printf("Push : Cross\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::RightUp) == ap::InputState::Push) printf("Push : Triangle\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::RightLeft) == ap::InputState::Push) printf("Push : Square\n");

				if (joystick->GetButtonState(0, ap::JoystickButtonType::L1) == ap::InputState::Push) printf("Push : L1\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::R1) == ap::InputState::Push) printf("Push : R1\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::L2) == ap::InputState::Push) printf("Push : L2\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::R2) == ap::InputState::Push) printf("Push : R2\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::L3) == ap::InputState::Push) printf("Push : L3\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::R3) == ap::InputState::Push) printf("Push : R3\n");

				if (joystick->GetButtonState(0, ap::JoystickButtonType::LeftUp) == ap::InputState::Push) printf("Push : Up\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::LeftDown) == ap::InputState::Push) printf("Push : Down\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::LeftLeft) == ap::InputState::Push) printf("Push : Left\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::LeftRight) == ap::InputState::Push) printf("Push : Right\n");

				if (joystick->GetButtonState(0, ap::JoystickButtonType::LeftStart) == ap::InputState::Push) printf("Push : LeftStart\n");
				if (joystick->GetButtonState(0, ap::JoystickButtonType::RightStart) == ap::InputState::Push) printf("Push : RightStart\n");

				auto leftH = joystick->GetAxisState(0, ap::JoystickAxisType::LeftH);
				auto leftV = joystick->GetAxisState(0, ap::JoystickAxisType::LeftV);
				auto rightH = joystick->GetAxisState(0, ap::JoystickAxisType::RightH);
				auto rightV = joystick->GetAxisState(0, ap::JoystickAxisType::RightV);

				auto l2 = joystick->GetAxisState(0, ap::JoystickAxisType::L2);
				auto r2 = joystick->GetAxisState(0, ap::JoystickAxisType::R2);

				if (std::abs(leftH) > 0.1f) printf("LeftH : %f\n", leftH);
				if (std::abs(leftV) > 0.1f) printf("LeftV : %f\n", leftV);
				if (std::abs(rightH) > 0.1f) printf("RightH : %f\n", rightH);
				if (std::abs(rightV) > 0.1f) printf("RightV : %f\n", rightV);

				//if (l2 > 0.5f) printf("L2 : %f\n", l2);
				//if (r2 > 0.5f) printf("R2 : %f\n", r2);
			}
		}
	}

	delete joystick;
	delete window;

	return 0;
}