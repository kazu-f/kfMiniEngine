#include "stdafx.h"
#include "RaceController.h"
#include "CheckPoint/CheckedController.h"
#include "RaceUI/LapUI.h"
#include "RaceUI/ReverseRunUI.h"
#include "RaceUI/GoalUI.h"

RaceController::RaceController()
{
}

RaceController::~RaceController()
{
}

void RaceController::OnDestroy()
{
	//後処理削除。
	DeleteGO(m_LapUI);
	DeleteGO(m_reverseRunUI);
	DeleteGO(m_goalUI);
}

bool RaceController::Start()
{
	//周回数のUI
	m_LapUI = NewGO<LapUI>(4);

	//逆走中表記のUI。
	m_reverseRunUI = NewGO<ReverseRunUI>(4);

	//ゴール表記のUI。
	m_goalUI = NewGO<GoalUI>(4);

	return true;
}

void RaceController::Update()
{
	if (m_player == nullptr) {
		return;
	}
	//ゴール済みかどうかを更新する。
	GoaledCarUpdate();
	//ラップ番号をセット。
	m_LapUI->SetCurrentLap(m_player->GetCurrentLapNum());
	//逆走中のフラグを設定する。
	m_reverseRunUI->SetIsReverseRunFlag(m_player->IsReverseRun());

	//ゴール時の処理。
	if (m_LapUI->IsGoal() && !m_goalUI->IsActive()) {
		//ゴール表記を有効にする。
		m_goalUI->ActiveGoal(m_goaledCars[0] == m_player);		//プレイヤーが1着かどうか？
	}
}

void RaceController::GoaledCarUpdate()
{
	//プレイヤーがゴール済みか判定。
	if (m_player->IsGoal())
	{
		RegistGoaledCar(m_player);
	}
	//CPUがゴール済みか判定。
	for (int i = 0; i < m_cpusNum; i++)
	{
		if (m_cpus[i]->IsGoal())
		{
			RegistGoaledCar(m_cpus[i]);
		}
	}
}

void RaceController::RegistGoaledCar(CheckedController* goaledCar)
{
	ENGINE_ASSERT(
		m_goaledCarsNum < CAR_NUM,
		"レース参加車の最大数を超えています。"
	);
	//登録済みかどうかの確認。
	for (int i = 0; i < m_goaledCarsNum; i++)
	{
		if (m_goaledCars[i] == goaledCar) {
			//登録済み。
			return;
		}
	}
	//登録を行う。
	m_goaledCars[m_goaledCarsNum] = goaledCar;
	m_goaledCarsNum++;
}

