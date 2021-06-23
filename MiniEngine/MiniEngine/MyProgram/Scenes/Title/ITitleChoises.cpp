#include "stdafx.h"
#include "ITitleChoises.h"

ITitleChoises::ITitleChoises(const char* filePath, const Vector2& size)
{
	//スプライトの読み込み。
	m_choisesSprite = NewGO<prefab::CSpriteRender>(7);
	m_choisesSprite->Init(
		filePath,
		size.x,
		size.y
	);
}

ITitleChoises::~ITitleChoises()
{
	//開放。
	DeleteGO(m_choisesSprite);
}

void ITitleChoises::Enter()
{
	SpriteColorWhite();
}

void ITitleChoises::Leave()
{
	SpriteColorGray();
}
