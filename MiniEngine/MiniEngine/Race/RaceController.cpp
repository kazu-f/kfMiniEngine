#include "stdafx.h"
#include "RaceController.h"
#include "CheckPoint/CheckedController.h"
#include "RaceUI/LapUI.h"

RaceController::RaceController()
{
}

RaceController::~RaceController()
{
}

void RaceController::OnDestroy()
{
}

bool RaceController::Start()
{
	//周回数のUI
	m_LapUI = NewGO<LapUI>(0);

	return true;
}

void RaceController::Update()
{
	if (m_player == nullptr) {
		return;
	}
	//ラップ番号をセット。
	m_LapUI->SetCurrentLap(m_player->GetCurrentLapNum());

	if (m_LapUI->IsGoal()) {

	}
}
