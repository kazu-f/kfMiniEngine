#pragma once
#include "ITitleChoises.h"

class ChoiseGameStart : public ITitleChoises
{
public:
	ChoiseGameStart(const char* filePath, const Vector2& size);
	~ChoiseGameStart();
	void Execute()override final;

private:
	const float END_SELECTED_TIME = 1.2f;
	float m_currentTime = 0.0f;
};

