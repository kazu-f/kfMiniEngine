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
	//�㏈���폜�B
	DeleteGO(m_LapUI);
	DeleteGO(m_reverseRunUI);
	DeleteGO(m_goalUI);
}

bool RaceController::Start()
{
	//���񐔂�UI
	m_LapUI = NewGO<LapUI>(4);

	//�t�����\�L��UI�B
	m_reverseRunUI = NewGO<ReverseRunUI>(4);

	//�S�[���\�L��UI�B
	m_goalUI = NewGO<GoalUI>(4);

	return true;
}

void RaceController::Update()
{
	if (m_player == nullptr) {
		return;
	}
	//���b�v�ԍ����Z�b�g�B
	m_LapUI->SetCurrentLap(m_player->GetCurrentLapNum());
	//�t�����̃t���O��ݒ肷��B
	m_reverseRunUI->SetIsReverseRunFlag(m_player->IsReverseRun());

	//�S�[�����̏����B
	if (m_LapUI->IsGoal()) {
		//�S�[���\�L��L���ɂ���B
		m_goalUI->ActiveGoal();
	}
}
