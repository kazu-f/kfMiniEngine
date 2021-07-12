#include "stdafx.h"
#include "RaceController.h"
#include "Car/Car.h"
#include "CheckPoint/CheckedController.h"
#include "RaceUI/LapUI.h"
#include "RaceUI/ReverseRunUI.h"
#include "RaceUI/GoalUI.h"
#include "RaceUI/RaceStartCountDown.h"

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
	DeleteGO(m_raceStart);
}

bool RaceController::Start()
{
	//���񐔂�UI
	m_LapUI = NewGO<LapUI>(4);

	//�t�����\�L��UI�B
	m_reverseRunUI = NewGO<ReverseRunUI>(4);

	//�S�[���\�L��UI�B
	m_goalUI = NewGO<GoalUI>(4);

	//�X�^�[�g�̃J�E���g�_�E�����s���N���X�B
	m_raceStart = NewGO<RaceStartCountDown>(4);
	m_raceStart->SetActiveFlag(false);

	return true;
}

void RaceController::Update()
{
	if (m_player == nullptr) {
		return;
	}
	m_raceStart->SetActiveFlag(m_isInRaceScene);
	if (!m_raceStart->IsRaceStarted()) {
		return; 
	}
	else {
		//���[�X�J�n�̍��}�𑗂�B
		m_player->SetIsRaceStart(true);
		for (int i = 0; i < m_cpusNum; i++) {
			m_cpus[i]->SetIsRaceStart(true);
		}
	}

	//�S�[���ς݂��ǂ������X�V����B
	GoaledCarUpdate();
	//���b�v�ԍ����Z�b�g�B
	m_LapUI->SetCurrentLap(m_player->GetCheckedController()->GetCurrentLapNum());
	//�t�����̃t���O��ݒ肷��B
	m_reverseRunUI->SetIsReverseRunFlag(m_player->GetCheckedController()->IsReverseRun());

	//�S�[�����̏����B
	if (m_LapUI->IsGoal() && !m_goalUI->IsActive()) {
		//�S�[���\�L��L���ɂ���B
		m_goalUI->ActiveGoal(m_goaledCars[0] == m_player);		//�v���C���[��1�����ǂ����H
	}
}

bool RaceController::IsEndRace()
{
	return m_goalUI->IsTimePassed();
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

void RaceController::RegistGoaledCar(Car* goaledCar)
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

