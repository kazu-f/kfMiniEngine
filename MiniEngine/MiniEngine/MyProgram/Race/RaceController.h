#pragma once
#include "RaceEnum.h"
class Car;
class LapUI;
class ReverseRunUI;
class GoalUI;
class RaceStartCountDown;

/// <summary>
/// ���[�X���Ǘ�����B
/// </summary>
class RaceController : public IGameObject
{
public:
	RaceController();
	~RaceController();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	//�v���C���[���w�肷��B
	void SetPlayer(Car* player)
	{
		m_player = player;
	}
	//CPU��o�^����B
	void RegistCPU(Car* cpus)
	{
		ENGINE_ASSERT(
			m_cpusNum < MAX_CPU_NUM,
			"CPU�̍ő吔�𒴂��Ă��܂��B"
		);
		m_cpus[m_cpusNum] = cpus;
		m_cpusNum++;
	}
	//���[�X�V�[���Ɉڍs�ł������B
	void SetInRaceScene(bool flag)
	{
		m_isInRaceScene = flag;
	}
	//���[�X���I���������H
	bool IsEndRace();

private:
	/// <summary>
	/// �S�[���ς݂̎Ԃ��X�V����B
	/// </summary>
	void GoaledCarUpdate();
	/// <summary>
	/// �S�[���ς݂̎Ԃ�o�^�B
	/// </summary>
	/// <param name="goaledCar">�S�[���ς݂̎ԁB</param>
	void RegistGoaledCar(Car* goaledCar);

public:
	static const int MAX_CPU_NUM = 5;
	static const int CAR_NUM = 6;

private:
	Car* m_player = nullptr;		//�v���C���[�B
	Car* m_cpus[MAX_CPU_NUM] = { nullptr };		//CPU�B
	int m_cpusNum = 0;					//CPU�̐��B
	Car* m_goaledCars[CAR_NUM] = { nullptr };	//�S�[���ς݂̎ԁB
	int m_goaledCarsNum = 0;					//�S�[���ς݂̎Ԃ̐��B
	RaceStartCountDown* m_raceStart = nullptr;	//���[�X�J�n�̃J�E���g�_�E�����s���N���X�B
	LapUI* m_LapUI = nullptr;					//���񐔂̕\�������s���N���X�B
	ReverseRunUI* m_reverseRunUI = nullptr;		//�t�����̕\�����s���N���X�B
	GoalUI* m_goalUI = nullptr;					//�S�[���\�����s���N���X�B
	int m_GoaledCarNum = 0;						//�S�[���ς݂̎Ԃ̐��B
	bool m_isInRaceScene = false;				//���[�X�V�[���Ɉڍs�����ł������B
};

