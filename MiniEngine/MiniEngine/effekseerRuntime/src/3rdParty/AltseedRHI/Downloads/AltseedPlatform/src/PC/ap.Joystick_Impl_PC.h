#pragma once

#include "../ap.Joystick.h"

namespace ap
{

	class Joystick_Impl_PC
		: public Joystick
	{
	private:
		static const int MAX_AXES_NUM = 10;
		static const int MAX_BUTTONS_NUM = 30;
		static const int MAX_JOYSTICKS_NUM = 16;

		Window*		window = nullptr;

		std::array<bool, MAX_JOYSTICKS_NUM>				isPresent;

		std::array<std::u16string, MAX_JOYSTICKS_NUM>	names;
		std::array<JoystickType, MAX_JOYSTICKS_NUM>		types;
		std::array<int32_t, MAX_JOYSTICKS_NUM>			buttonCount;
		std::array<int32_t, MAX_JOYSTICKS_NUM>			axisCount;

		std::array<std::array<bool,MAX_BUTTONS_NUM>, MAX_JOYSTICKS_NUM>	currentHit;
		std::array<std::array<bool,MAX_BUTTONS_NUM>, MAX_JOYSTICKS_NUM>	preHit;

		std::array<std::array<float, MAX_AXES_NUM>, MAX_JOYSTICKS_NUM>	currentAxis;

	public:
		Joystick_Impl_PC(Window* window);
		virtual ~Joystick_Impl_PC();

		void RefreshInputState() override;

		void RefreshConnectedState() override;

		bool IsPresent(int32_t joystickIndex) override;

		const char16_t* GetName(int32_t joystickIndex) const override;

		InputState GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const override;

		InputState GetButtonState(int32_t joystickIndex, JoystickButtonType type) const override;

		float GetAxisState(int32_t joystickIndex, int32_t axisIndex) const override;

		float GetAxisState(int32_t joystickIndex, JoystickAxisType type) const override;

		JoystickType GetJoystickType(int32_t joystickIndex) const override;

		int32_t GetButtonCount(int32_t joystickIndex) const override;

		int32_t GetAxisCount(int32_t joystickIndex) const override;
	};

}