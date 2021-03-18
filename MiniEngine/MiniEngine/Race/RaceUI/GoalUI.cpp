#include "stdafx.h"
#include "GoalUI.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/RaceGoalUI.dds";				//スプライトのファイルパス。

	const Vector3 SPRITE_POS = { 0.0f,120.0f,0.0f };				//スプライトの位置。
	const Vector2 SPRITE_SIZE = { 300.0f,100.0f };					//スプライトのサイズ。
	const Vector2 SPRITE_PIVOT = { 0.0f, 0.0f };				//スプライトのピボット。

	const int SPRITE_PRIO = 5;									//スプライトの実行優先度。
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

	//まだ有効にしない。
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
	//時間を経過させる。
	m_currentElapsedTime += GameTime().GetFrameDeltaTime();
}

