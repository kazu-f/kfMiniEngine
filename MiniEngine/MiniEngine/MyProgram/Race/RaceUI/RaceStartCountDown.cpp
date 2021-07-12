#include "stdafx.h"
#include "RaceStartCountDown.h"

namespace {
	const char* COUNT_DOWN_FILEPATH[RaceStartCountDown::EnCountDown::enNum] = {
		"Assets/sprite/Race/LapNum_3.dds",
		"Assets/sprite/Race/LapNum_2.dds",
		"Assets/sprite/Race/LapNum_1.dds",
		"Assets/sprite/Race/StartGO.dds"
	};
	const wchar_t* SE_COUNT_FILEPATH = L"Assets/sound/Race/SECount.wav";
	const wchar_t* SE_START_FILEPATH = L"Assets/sound/Race/SEStart.wav";

	Vector2 SPRITE_SIZE[RaceStartCountDown::EnCountDown::enNum] = {
		{200.0f,200.0f},
		{200.0f,200.0f},
		{200.0f,200.0f},
		{400.0f,200.0f}
	};

	Vector2 SPRITE_POS = { 0.0f,230.0f };

	const float WAIT_TIME = 1.8f;
	const float COUNT_FADE_START_TIME = 0.7f;
	const float COUNT_FADE_END_TIME = 1.0f;

	const float COUNT_SCALE_START = 1.0f;
	const float COUNT_SCALE_END = 0.8f;
}

RaceStartCountDown::RaceStartCountDown()
{
}

RaceStartCountDown::~RaceStartCountDown()
{
}

bool RaceStartCountDown::Start()
{
	for (int i = 0;i<EnCountDown::enNum;i++)
	{
		//スプライトを読み込む。
		m_countDownSprite[i] = NewGO<prefab::CSpriteRender>(10);
		m_countDownSprite[i]->Init(
			COUNT_DOWN_FILEPATH[i],
			SPRITE_SIZE[i].x,
			SPRITE_SIZE[i].y
		);

		m_countDownSprite[i]->SetPosition(SPRITE_POS);
		//表示はまだしない。
		m_countDownSprite[i]->SetActiveFlag(false);

		//SEを読み込む。
		m_seCount[i] = NewGO<prefab::CSoundSource>(0);
		if (i != en_start) {
			//カウントダウンのSE。
			m_seCount[i]->Init(SE_COUNT_FILEPATH);
		}
		else {
			//スタートのSE。
			m_seCount[i]->Init(SE_START_FILEPATH);
		}
	}

	return true;
}

void RaceStartCountDown::OnDestroy()
{
	for (auto* sprite : m_countDownSprite)
	{
		DeleteGO(sprite);
	}
	for (auto* se : m_seCount)
	{
		DeleteGO(se);
	}
}

void RaceStartCountDown::Update()
{
	const float DelTime = GameTime().GetFrameDeltaTime();
	if (!WaitCountEnable(DelTime))return;
	//SEを鳴らす。
	CountSound();
	//スプライトを表示する。
	CountFade(DelTime);
}

void RaceStartCountDown::CountFade(const float time)
{
	if (m_countState == enNum) return;
	//スプライトを有効にする
	m_countDownSprite[m_countState]->SetActiveFlag(true);

	m_timeInSecond += time;

	float currentTime = (m_timeInSecond - static_cast<int>(m_countState));
	//スケールを下げていく。
	if (m_countState != en_start) {
		float scaleSize = COUNT_SCALE_START * (1.0f - currentTime) + COUNT_SCALE_END * currentTime;
		m_countDownSprite[m_countState]->SetScale({ scaleSize ,scaleSize ,scaleSize });
	}

	//経過時間からα値を計算。
	float alphaWeight = (COUNT_FADE_END_TIME - currentTime) / (COUNT_FADE_END_TIME - COUNT_FADE_START_TIME);
	alphaWeight = min(1.0f, max(0.0f, alphaWeight));
	m_countDownSprite[m_countState]->SetMulColor({ 1.0f,1.0f,1.0f,alphaWeight });

	if (m_countState + 1 <= static_cast<int>(m_timeInSecond))
	{
		m_countDownSprite[m_countState]->SetActiveFlag(false);
		m_countState++;
		m_countUpNow = true;
	}
}

bool RaceStartCountDown::WaitCountEnable(const float time)
{
	m_timeIsWait += time;
	
	return m_timeIsWait > WAIT_TIME;
}

void RaceStartCountDown::CountSound()
{
	if (m_countState == enNum) return;
	if (!m_countUpNow)return;
	m_seCount[m_countState]->Play(false);
	m_seCount[m_countState] = nullptr;
	m_countUpNow = false;

}
