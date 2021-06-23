#include "stdafx.h"
#include "ITitleChoises.h"

ITitleChoises::ITitleChoises(const char* filePath, const Vector2& size)
{
	//�X�v���C�g�̓ǂݍ��݁B
	m_choisesSprite = NewGO<prefab::CSpriteRender>(7);
	m_choisesSprite->Init(
		filePath,
		size.x,
		size.y
	);
}

ITitleChoises::~ITitleChoises()
{
	//�J���B
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
