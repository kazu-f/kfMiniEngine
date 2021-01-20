#pragma once

#include "../ap.Mouse.h"

namespace ap
{

	class Mouse_Impl_PC
		: public Mouse
	{
	private:
		static const int KEY_NUM = 8;

		Window*		window = nullptr;

		std::array<bool, KEY_NUM>	currentHit;
		std::array<bool, KEY_NUM>	preHit;
		float	wheel = 0;

	public:
		Mouse_Impl_PC(Window* window);
		virtual ~Mouse_Impl_PC();

		void RefreshInputState() override;

		void SetPosition(float x, float y) override;

		void SetWheelCallback(std::function<void(double x, double y)> func) override;

		float GetWheel() const override;

		InputState GetMouseButtonState(MouseButtons button) const override;
	};

}