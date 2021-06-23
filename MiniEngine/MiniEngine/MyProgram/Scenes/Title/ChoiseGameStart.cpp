#include "stdafx.h"
#include "ChoiseGameStart.h"
#include "Scenes\GameScene.h"

ChoiseGameStart::ChoiseGameStart(const char* filePath, const Vector2& size)
	:ITitleChoises::ITitleChoises(filePath, size)
{
}

ChoiseGameStart::~ChoiseGameStart()
{
}

void ChoiseGameStart::Execute()
{
	m_currentTime += GameTime().GetFrameDeltaTime();
	if (m_currentTime > END_SELECTED_TIME)
	{
		NewGO<GameScene>(0);
		m_isSelected = true;
	}
}
