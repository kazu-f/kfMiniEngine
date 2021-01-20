#pragma once

#include "ap.Base.h"

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