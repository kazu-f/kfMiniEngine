#include "stdafx.h"
#include "ReverseRunUI.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/RoadClosed.dds";	//スプライトのファイルパス。
	const Vector3 UI_POS = { 0.0f,200.0f,0.0f };				//スプライトの位置。
	const Vector2 UI_SIZE = { 100.0f,100.0f };				//スプライトのサイズ。

	const int SPRITE_PRIO = 5;									//スプライトの実行優先度。

	const float INTARVAL_TIME = 0.4f;						//点滅間隔時間。
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
	//スプライトの初期化。
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
	//時間経過。
	m_currentTime += GameTime().GetFrameDeltaTime();

	//時間を間隔で割る。
	float flagTime = m_currentTime / INTARVAL_TIME;
	//整数部を求める。
	int count = static_cast<int>(floorf(flagTime));
	//整数部の偶数奇数でアクティブフラグを切り替える。
	int activeFlag = count % 2;
	m_sprite->SetActiveFlag(activeFlag == 1);
}
