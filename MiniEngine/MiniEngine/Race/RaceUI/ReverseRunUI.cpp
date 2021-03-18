#include "stdafx.h"
#include "ReverseRunUI.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/RoadClosed.dds";	//�X�v���C�g�̃t�@�C���p�X�B
	const Vector3 UI_POS = { 0.0f,200.0f,0.0f };				//�X�v���C�g�̈ʒu�B
	const Vector2 UI_SIZE = { 100.0f,100.0f };				//�X�v���C�g�̃T�C�Y�B

	const int SPRITE_PRIO = 5;									//�X�v���C�g�̎��s�D��x�B

	const float INTARVAL_TIME = 0.4f;						//�_�ŊԊu���ԁB
}

ReverseRunUI::ReverseRunUI()
{
}

ReverseRunUI::~ReverseRunUI()
{
}
void ReverseRunUI::OnDestroy()
{
	DeleteGO(m_sprite);
}

bool ReverseRunUI::Start()
{
	//�X�v���C�g�̏������B
	m_sprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_sprite->Init(SPRITE_FILEPATH, UI_SIZE.x, UI_SIZE.y);
	m_sprite->SetPosition(UI_POS);

	return true;
}

void ReverseRunUI::Update()
{
	if (m_isReverseRun) {
		ReverseRun();
	}
	else {
		m_sprite->SetActiveFlag(false);
		m_currentTime = 0.0f;
	}
}


void ReverseRunUI::ReverseRun()
{
	//���Ԍo�߁B
	m_currentTime += GameTime().GetFrameDeltaTime();

	//���Ԃ��Ԋu�Ŋ���B
	float flagTime = m_currentTime / INTARVAL_TIME;
	//�����������߂�B
	int count = static_cast<int>(floorf(flagTime));
	//�������̋�����ŃA�N�e�B�u�t���O��؂�ւ���B
	int activeFlag = count % 2;
	m_sprite->SetActiveFlag(activeFlag == 1);
}
