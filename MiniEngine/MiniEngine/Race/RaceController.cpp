#include "stdafx.h"
#include "RaceController.h"
#include "CheckPoint/CheckedController.h"
#include "RaceUI/LapUI.h"
#include "RaceUI/ReverseRunUI.h"

RaceController::RaceController()
{
}

RaceController::~RaceController()
{
}

void RaceController::OnDestroy()
{
	DeleteGO(m_LapUI);
	DeleteGO(m_reverseRunUI);
}

bool RaceController::Start()
{
	//周回数のUI
	m_LapUI = NewGO<LapUI>(4);
	//逆走中表記のUI。
	m_reverseRunUI = NewGO<ReverseRunUI>(4);

	return true;
}

void RaceController::Update()
{
	if (m_player == nullptr) {
		return;
	}
	//ラップ番号をセット。
	m_LapUI->SetCurrentLap(m_player->GetCurrentLapNum());
	//逆走中のフラグを設定する。
	m_reverseRunUI->SetIsReverseRunFlag(m_player->IsReverseRun());

	if (m_LapUI->IsGoal()) {

	}
}
