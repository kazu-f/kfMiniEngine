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
	//�S�[���ς݂��ǂ������X�V����B
	GoaledCarUpdate();
	//���b�v�ԍ����Z�b�g�B
	m_LapUI->SetCurrentLap(m_player->GetCurrentLapNum());
	//�t�����̃t���O��ݒ肷��B
	m_reverseRunUI->SetIsReverseRunFlag(m_player->IsReverseRun());

	//�S�[�����̏����B
	if (m_LapUI->IsGoal() && !m_goalUI->IsActive()) {
		//�S�[���\�L��L���ɂ���B
		m_goalUI->ActiveGoal(m_goaledCars[0] == m_player);		//�v���C���[��1�����ǂ����H
	}
}

void RaceController::GoaledCarUpdate()
{
	//�v���C���[���S�[���ς݂�����B
	if (m_player->IsGoal())
	{
		RegistGoaledCar(m_player);
	}
	//CPU���S�[���ς݂�����B
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
		"���[�X�Q���Ԃ̍ő吔�𒴂��Ă��܂��B"
	);
	//�o�^�ς݂��ǂ����̊m�F�B
	for (int i = 0; i < m_goaledCarsNum; i++)
	{
		if (m_goaledCars[i] == goaledCar) {
			//�o�^�ς݁B
			return;
		}
	}
	//�o�^���s���B
	m_goaledCars[m_goaledCarsNum] = goaledCar;
	m_goaledCarsNum++;
}

