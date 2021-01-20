
#include "ap.Joystick_Impl_PC.h"

#include <string.h>
#include <codecvt>
#include <locale>

namespace ap
{

// http://hasenpfote36.blogspot.jp/2016/09/stdcodecvt.html
static constexpr std::codecvt_mode mode = std::codecvt_mode::little_endian;

static std::u16string utf8_to_utf16(const std::string& s)
{
#if defined(_MSC_VER)
	std::wstring_convert<std::codecvt_utf8_utf16<std::uint16_t, 0x10ffff, mode>, std::uint16_t> conv;
	auto temp = conv.from_bytes(s);
	return std::u16string(temp.cbegin(), temp.cend());
#else
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, mode>, char16_t> conv;
	return conv.from_bytes(s);
#endif
}

Joystick_Impl_PC::Joystick_Impl_PC(Window* window)
{
	isPresent.fill(false);
	names.fill(u"");
	types.fill(JoystickType::Other);
	buttonCount.fill(0);
	axisCount.fill(0);
	
	for (int32_t i = 0; i < MAX_JOYSTICKS_NUM; i++)
	{
		currentHit[i].fill(false);
		preHit[i].fill(false);
		currentAxis[i].fill(0);
	}

	RefreshConnectedState();
	RefreshInputState();
}

Joystick_Impl_PC::~Joystick_Impl_PC()
{

}

void Joystick_Impl_PC::RefreshInputState()
{
	preHit = currentHit;

	for (int jind = 0; jind < MAX_JOYSTICKS_NUM; jind++)
	{
		int32_t axesCount = 0;
		auto ax = glfwGetJoystickAxes(jind, &axesCount);
		for (int i = 0; i < axesCount; ++i)
		{
			currentAxis[jind][i] = ax[i];
		}

		int32_t buttonsCount = 0;
		auto btns = glfwGetJoystickButtons(jind, &buttonsCount);

		for (int i = 0; i < buttonsCount; ++i)
		{
			currentHit[jind][i] = (bool)btns[i];
		}

		auto jtype = GetJoystickType(jind);

		if (jtype == JoystickType::XBOX360)
		{
			currentHit[jind][14] = currentAxis[jind][4] > 0.8;
			currentHit[jind][15] = currentAxis[jind][5] > 0.8;
		}

		buttonCount[jind] = buttonsCount;
		axisCount[jind] = axesCount;
	}
}

void Joystick_Impl_PC::RefreshConnectedState()
{
	for (int i = 0; i < MAX_JOYSTICKS_NUM; i++)
	{
		isPresent[i] = glfwJoystickPresent(i) == GLFW_TRUE;

		if (!isPresent[i]) continue;

		// Recognize joystick
		auto name = glfwGetJoystickName(i);

		names[i] = utf8_to_utf16(name);

		int32_t buttonsCount = 0;
		auto btns = glfwGetJoystickButtons(i, &buttonsCount);

		if (strcmp(name, "Wireless Controller") == 0 && buttonsCount == 18)
		{
			types[i] = JoystickType::PS4;
		}
		else if (strcmp(name, "Xbox 360 Controller") == 0 && buttonsCount == 14)
		{
			types[i] = JoystickType::XBOX360;
		}
		//else if (strcmp(name, "Controller") == 0 && buttonsCount == 15)
		//{
		//	types[i] = JoystickType::XBOX;
		//}
	}
}

bool Joystick_Impl_PC::IsPresent(int32_t joystickIndex)
{
	if (joystickIndex < 0) return false;
	if (joystickIndex >= MAX_JOYSTICKS_NUM) return false;

	return isPresent[joystickIndex];
}

const char16_t* Joystick_Impl_PC::GetName(int32_t joystickIndex) const
{
	return names[joystickIndex].c_str();
}

InputState Joystick_Impl_PC::GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const
{
	if (buttonIndex < 0) return InputState::Free;

	if (currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return InputState::Hold;
	else if (!currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return InputState::Release;
	else if (currentHit[joystickIndex][buttonIndex] && !preHit[joystickIndex][buttonIndex]) return InputState::Push;
	else return InputState::Free;
}

InputState Joystick_Impl_PC::GetButtonState(int32_t joystickIndex, JoystickButtonType type) const
{
	auto jtype = GetJoystickType(joystickIndex);
	if (jtype == JoystickType::Other) return InputState::Free;

	if (jtype == JoystickType::PS4)
	{
		std::array<int, (int32_t)JoystickButtonType::Max> maps;
		maps.fill(-1);

		maps[(int32_t)JoystickButtonType::RightLeft] = 0;
		maps[(int32_t)JoystickButtonType::RightDown] = 1;
		maps[(int32_t)JoystickButtonType::RightRight] = 2;
		maps[(int32_t)JoystickButtonType::RightUp] = 3;

		maps[(int32_t)JoystickButtonType::L1] = 4;
		maps[(int32_t)JoystickButtonType::R1] = 5;
		maps[(int32_t)JoystickButtonType::L2] = 6;
		maps[(int32_t)JoystickButtonType::R2] = 7;
		maps[(int32_t)JoystickButtonType::L3] = 10;
		maps[(int32_t)JoystickButtonType::R3] = 11;

		maps[(int32_t)JoystickButtonType::LeftUp] = 14;
		maps[(int32_t)JoystickButtonType::LeftRight] = 15;
		maps[(int32_t)JoystickButtonType::LeftDown] = 16;
		maps[(int32_t)JoystickButtonType::LeftLeft] = 17;

		maps[(int32_t)JoystickButtonType::LeftStart] = 13;
		maps[(int32_t)JoystickButtonType::RightStart] = 9;

		return GetButtonState(joystickIndex, maps[(int32_t)type]);
	}

	if (jtype == JoystickType::XBOX360)
	{
		std::array<int, (int32_t)JoystickButtonType::Max> maps;
		maps.fill(-1);

		maps[(int32_t)JoystickButtonType::RightLeft] = 2;
		maps[(int32_t)JoystickButtonType::RightDown] = 0;
		maps[(int32_t)JoystickButtonType::RightRight] = 1;
		maps[(int32_t)JoystickButtonType::RightUp] = 3;

		maps[(int32_t)JoystickButtonType::L1] = 4;
		maps[(int32_t)JoystickButtonType::R1] = 5;

		maps[(int32_t)JoystickButtonType::L2] = 14;
		maps[(int32_t)JoystickButtonType::R2] = 15;

		maps[(int32_t)JoystickButtonType::L3] = 8;
		maps[(int32_t)JoystickButtonType::R3] = 9;

		maps[(int32_t)JoystickButtonType::LeftUp] = 10;
		maps[(int32_t)JoystickButtonType::LeftRight] = 11;
		maps[(int32_t)JoystickButtonType::LeftDown] = 12;
		maps[(int32_t)JoystickButtonType::LeftLeft] = 13;

		maps[(int32_t)JoystickButtonType::LeftStart] = 6;
		maps[(int32_t)JoystickButtonType::RightStart] = 7;

		return GetButtonState(joystickIndex, maps[(int32_t)type]);
	}

	return InputState::Free;
}

float Joystick_Impl_PC::GetAxisState(int32_t joystickIndex, int32_t axisIndex) const
{
	if (axisIndex < 0) return 0;

	return currentAxis[joystickIndex][axisIndex];
}

float Joystick_Impl_PC::GetAxisState(int32_t joystickIndex, JoystickAxisType type) const
{
	auto jtype = GetJoystickType(joystickIndex);
	if (jtype == JoystickType::Other) return 0;

	if (jtype == JoystickType::PS4)
	{
		std::array<int, (int32_t)JoystickAxisType::Max> maps;
		maps.fill(-1);

		maps[(int32_t)JoystickAxisType::LeftH] = 0;
		maps[(int32_t)JoystickAxisType::LeftV] = 1;
		maps[(int32_t)JoystickAxisType::RightH] = 2;
		maps[(int32_t)JoystickAxisType::RightV] = 5;

		maps[(int32_t)JoystickAxisType::L2] = 3;
		maps[(int32_t)JoystickAxisType::R2] = 4;

		return GetAxisState(joystickIndex, maps[(int32_t)type]);
	}

	if (jtype == JoystickType::XBOX360)
	{
		std::array<int, (int32_t)JoystickAxisType::Max> maps;
		maps.fill(-1);

		maps[(int32_t)JoystickAxisType::LeftH] = 0;
		maps[(int32_t)JoystickAxisType::LeftV] = 1;
		maps[(int32_t)JoystickAxisType::RightH] = 2;
		maps[(int32_t)JoystickAxisType::RightV] = 3;

		maps[(int32_t)JoystickAxisType::L2] = 4;
		maps[(int32_t)JoystickAxisType::R2] = 5;

		if ((int32_t)type == 1 || (int32_t)type == 3)
		{
			return -GetAxisState(joystickIndex, maps[(int32_t)type]);
		}

		return GetAxisState(joystickIndex, maps[(int32_t)type]);
	}



	return 0.0f;
}

JoystickType Joystick_Impl_PC::GetJoystickType(int32_t joystickIndex) const
{
	return types[joystickIndex];
}

int32_t Joystick_Impl_PC::GetButtonCount(int32_t joystickIndex) const
{
	return buttonCount[joystickIndex];
}

int32_t Joystick_Impl_PC::GetAxisCount(int32_t joystickIndex) const
{
	return axisCount[joystickIndex];
}

}