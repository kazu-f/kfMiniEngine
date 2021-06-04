#include "stdafx.h"
#include "LapUI.h"

namespace {
	const Vector3 TEXTPOS = { 360.0f,-320.0f,0.0f };				//テキストの位置。
	const Vector2 TEXTSIZE = { 200.0f,100.0f };				//テキストのサイズ。

	//周回数のスプライトのファイルパス。
	const char* LAPNUM_FILEPATH[LapUI::enLap_Num] = {
		"Assets/sprite/LapNum_1.dds",
		"Assets/sprite/LapNum_2.dds",
		"Assets/sprite/LapNum_3.dds"
	};
	const char* LAPTEXT_FILEPATH = "Assets/sprite/LAP.dds";				//テキストが書かれたスプライト(LAP)
	const char* LAPSLASH_FILEPATH = "Assets/sprite/LapSlash.dds";		//スラッシュ(/)のスプライト。

	const Vector3 CURRENT_LAPPOS = { 480.0f,-320.0f,0.0f };		//現在の周回数の位置。
	const Vector3 MAX_LAPPOS = { 580.0f,-320.0f,0.0f };			//最大周回数の位置。
	const Vector3 SLASH_POS = { 530.0f,-320.0f,0.0f };			//スラッシュの位置。
	const Vector2 LAPNUM_SIZE = { 100.0f,100.0f };			//周回数のサイズ。

	const int SPRITE_PRIO = 5;									//スプライトの実行優先度。

	const float INTARVAL_TIME = 0.4f;						//点滅間隔時間。
	const float MAX_TIME = 2.0f;							//点滅時間。
}

LapUI::LapUI()
{
}

LapUI::~LapUI()
{
}

void LapUI::OnDestroy()
{
	//後処理削除。
	DeleteGO(m_lapTextSprite);
	DeleteGO(m_maxLapSprite);
	DeleteGO(m_slashSprite);
	for (int i = 0; i < enLap_Num; i++) {
		DeleteGO(m_currentLapSprite[i]);
	}
}

bool LapUI::Start()
{
	//テキスト。
	m_lapTextSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_lapTextSprite->Init(LAPTEXT_FILEPATH, TEXTSIZE.x, TEXTSIZE.y);
	m_lapTextSprite->SetPosition(TEXTPOS);
	//スラッシュ。
	m_slashSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_slashSprite->Init(LAPSLASH_FILEPATH, LAPNUM_SIZE.x, LAPNUM_SIZE.y);
	m_slashSprite->SetPosition(SLASH_POS);
	//最大周回数。
	m_maxLapSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_maxLapSprite->Init(LAPNUM_FILEPATH[enLap_Num - 1], LAPNUM_SIZE.x, LAPNUM_SIZE.y);
	m_maxLapSprite->SetPosition(MAX_LAPPOS);
	//現在周回数。
	for (int i = 0; i < enLap_Num; i++) {
		m_currentLapSprite[i] = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
		m_currentLapSprite[i]->Init(LAPNUM_FILEPATH[i], LAPNUM_SIZE.x, LAPNUM_SIZE.y);
		m_currentLapSprite[i]->SetPosition(CURRENT_LAPPOS);
		m_currentLapSprite[i]->SetActiveFlag(false);
	}
	//現在周回数だけ有効にする。
	m_currentLapSprite[m_currentLapNum]->SetActiveFlag(true);

	return true;
}

void LapUI::Update()
{
	//周回数が増えていない。
	if (!(m_currentLapNum < m_nextLapNum)) {
		return;
	}
	if (m_nextLapNum >= enLap_Num) {
		//ゴールした。
		m_isGoal = true;
	}
	else {
		//周回数を切り替える。。
		CountUpLap();
	}

}

void LapUI::CountUpLap()
{
	m_currentLapSprite[m_currentLapNum]->SetActiveFlag(false);

	//時間経過。
	m_currentTime += GameTime().GetFrameDeltaTime();

	if (m_currentTime > MAX_TIME) {
		m_currentLapSprite[m_nextLapNum]->SetActiveFlag(true);
		m_currentLapNum = m_nextLapNum;	//カウントを上げる。
		m_currentTime = 0.0f;			//元に戻す。
	}
	else {
		//時間を間隔で割る。
		float flagTime = m_currentTime / INTARVAL_TIME;
		//整数部を求める。
		int count = static_cast<int>(floorf(flagTime));
		//整数部の偶数奇数でアクティブフラグを切り替え。
		int activeFlag = count % 2;
		m_currentLapSprite[m_nextLapNum]->SetActiveFlag(activeFlag == 1);
	}
}
