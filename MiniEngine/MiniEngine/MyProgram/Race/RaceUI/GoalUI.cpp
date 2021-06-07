#include "stdafx.h"
#include "GoalUI.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/RaceGoalUI.dds";				//スプライトのファイルパス。
	const char* SPRITE_WIN_FILEPATH = "Assets/sprite/Win.dds";					//勝利表記のファイルパス。
	const char* SPRITE_LOSE_FILEPATH = "Assets/sprite/Lose.dds";					//敗北表記のファイルパス。

	const Vector3 SPRITE_POS = { 0.0f,120.0f,0.0f };				//スプライトの位置。
	const Vector2 SPRITE_SIZE = { 300.0f,100.0f };					//スプライトのサイズ。
	const Vector2 SPRITE_PIVOT = { 0.0f, 0.0f };				//スプライトのピボット。

	const int SPRITE_PRIO = 5;									//スプライトの実行優先度。

	const Vector2 RESULT_SIZE = { 256.0f,64.0f };				//リザルトのスプライトのサイズ。
	const Vector3 RESULT_POS = { 0.0f,-60.0f,0.0f };			//リザルトのスプライトの位置。
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
	//勝利表記。
	m_win = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_win->Init(SPRITE_WIN_FILEPATH, RESULT_SIZE.x, RESULT_SIZE.y);
	m_win->SetPosition(RESULT_POS);
	//敗北表記。
	m_lose = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_lose->Init(SPRITE_LOSE_FILEPATH, RESULT_SIZE.x, RESULT_SIZE.y);
	m_lose->SetPosition(RESULT_POS);


	//まだ有効にしない。
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
	//時間を経過させる。
	m_currentElapsedTime += GameTime().GetFrameDeltaTime();
}

