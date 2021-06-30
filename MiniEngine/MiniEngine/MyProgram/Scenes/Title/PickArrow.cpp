#include "stdafx.h"
#include "PickArrow.h"

namespace {
	const char* PICK_PATH = "Assets/sprite/Title/Race_ChoisePick.dds";		//矢印のスプライト。
	const Vector2 PICK_SIZE = { 100.0f,50.0f };				//矢印のスプライトのサイズ。
	const Vector2 PICK_PIVOT = { 1.0f,0.5f };					//矢印のスプライトのピボットの位置。
}

PickArrow::PickArrow()
{
}

PickArrow::~PickArrow()
{
}

void PickArrow::OnDestroy()
{
	DeleteGO(m_pickSprite);
}

bool PickArrow::Start()
{
	//矢印のスプライト。
	m_pickSprite = NewGO<prefab::CSpriteRender>(5);
	m_pickSprite->Init(
		PICK_PATH,
		PICK_SIZE.x,
		PICK_SIZE.y
	);
	m_pickSprite->SetPivot(PICK_PIVOT);			//ピボットの位置を調整。
	m_pickSprite->SetPosition(m_position);	//選択肢を合わせる。

	return true;
}

void PickArrow::Update()
{
	//時間を経過させる。
	m_swingTime += GameTime().GetFrameDeltaTime() * SWING_SPEED[m_state];
	float t = sinf(m_swingTime) / 2.0f + 0.5f;		//sin波を0.0～1.0に補完して揺らす。
	m_pickSprite->SetPosition(m_position + m_pickSwing * t);
}
