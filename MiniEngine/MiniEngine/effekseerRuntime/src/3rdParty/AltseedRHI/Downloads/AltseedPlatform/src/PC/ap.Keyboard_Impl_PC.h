#pragma once

#include "../ap.Keyboard.h"

namespace ap
{

class Keyboard_Impl_PC
	: public Keyboard
{
private:
	static const int KEY_NUM = 121;
	static const int keyCodes[128];

	Window*						window = nullptr;
	std::array<bool, KEY_NUM>	currentHit;
	std::array<bool, KEY_NUM>	preHit;

public:
	Keyboard_Impl_PC(Window* window);
	virtual ~Keyboard_Impl_PC();

	void RefreshInputState() override;

	InputState GetKeyState(Keys key) const override;
};

}