#include "stdafx.h"
#include "GoalUI.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/RaceGoalUI.dds";				//�X�v���C�g�̃t�@�C���p�X�B
	const char* SPRITE_WIN_FILEPATH = "Assets/sprite/Win.dds";					//�����\�L�̃t�@�C���p�X�B
	const char* SPRITE_LOSE_FILEPATH = "Assets/sprite/Lose.dds";					//�s�k�\�L�̃t�@�C���p�X�B

	const Vector3 SPRITE_POS = { 0.0f,120.0f,0.0f };				//�X�v���C�g�̈ʒu�B
	const Vector2 SPRITE_SIZE = { 300.0f,100.0f };					//�X�v���C�g�̃T�C�Y�B
	const Vector2 SPRITE_PIVOT = { 0.0f, 0.0f };				//�X�v���C�g�̃s�{�b�g�B

	const int SPRITE_PRIO = 5;									//�X�v���C�g�̎��s�D��x�B

	const Vector2 RESULT_SIZE = { 256.0f,64.0f };				//���U���g�̃X�v���C�g�̃T�C�Y�B
	const Vector3 RESULT_POS = { 0.0f,-60.0f,0.0f };			//���U���g�̃X�v���C�g�̈ʒu�B
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
	//�����\�L�B
	m_win = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_win->Init(SPRITE_WIN_FILEPATH, RESULT_SIZE.x, RESULT_SIZE.y);
	m_win->SetPosition(RESULT_POS);
	//�s�k�\�L�B
	m_lose = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_lose->Init(SPRITE_LOSE_FILEPATH, RESULT_SIZE.x, RESULT_SIZE.y);
	m_lose->SetPosition(RESULT_POS);


	//�܂��L���ɂ��Ȃ��B
	this->SetActiveFlag(false);
	m_sprite->SetActiveFlag(false);
	m_win->SetActiveFlag(false);
	m_lose->SetActiveFlag(false);

	return true;
}

void GoalUI::OnDestroy()
{
	DeleteGO(m_sprite);
	DeleteGO(m_win);
	DeleteGO(m_lose);
}

void GoalUI::Update()
{
	//���Ԃ��o�߂�����B
	m_currentElapsedTime += GameTime().GetFrameDeltaTime();
}

