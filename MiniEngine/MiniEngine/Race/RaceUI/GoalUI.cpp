#include "stdafx.h"
#include "GoalUI.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/RaceGoalUI.dds";				//�X�v���C�g�̃t�@�C���p�X�B

	const Vector3 SPRITE_POS = { 0.0f,120.0f,0.0f };				//�X�v���C�g�̈ʒu�B
	const Vector2 SPRITE_SIZE = { 300.0f,100.0f };					//�X�v���C�g�̃T�C�Y�B
	const Vector2 SPRITE_PIVOT = { 0.0f, 0.0f };				//�X�v���C�g�̃s�{�b�g�B

	const int SPRITE_PRIO = 5;									//�X�v���C�g�̎��s�D��x�B
}

const float GoalUI::TIME_LIMIT = 5.0f;

GoalUI::GoalUI()
{
}

GoalUI::~GoalUI()
{
}

bool GoalUI::Start()
{
	m_sprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_sprite->Init(SPRITE_FILEPATH, SPRITE_SIZE.x, SPRITE_SIZE.y);
	m_sprite->SetPosition(SPRITE_POS);

	//�܂��L���ɂ��Ȃ��B
	this->SetActiveFlag(false);
	m_sprite->SetActiveFlag(false);

	return true;
}

void GoalUI::OnDestroy()
{
	DeleteGO(m_sprite);
}

void GoalUI::Update()
{
	//���Ԃ��o�߂�����B
	m_currentElapsedTime += GameTime().GetFrameDeltaTime();
}

