#pragma once
#include "RaceEnum.h"
class CheckedController;
class LapUI;
class ReverseRunUI;
class GoalUI;

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
	void SetPlayer(CheckedController* player)
	{
		m_player = player;
	}
	//CPU��o�^����B
	void RegistCPU(CheckedController* cpus)
	{
		ENGINE_ASSERT(
			m_cpusNum < MAX_CPU_NUM,
			"CPU�̍ő吔�𒴂��Ă��܂��B"
		);
		m_cpus[m_cpusNum] = cpus;
		m_cpusNum++;
	}

private:
	/// <summary>
	/// �S�[���ς݂̎Ԃ��X�V����B
	/// </summary>
	void GoaledCarUpdate();
	/// <summary>
	/// �S�[���ς݂̎Ԃ�o�^�B
	/// </summary>
	/// <param name="goaledCar">�S�[���ς݂̎ԁB</param>
	void RegistGoaledCar(CheckedController* goaledCar);

public:
	static const int MAX_CPU_NUM = 5;
	static const int CAR_NUM = 6;

private:
	CheckedController* m_player = nullptr;		//�v���C���[�B
	CheckedController* m_cpus[MAX_CPU_NUM] = { nullptr };		//CPU�B
	int m_cpusNum = 0;					//CPU�̐��B
	CheckedController* m_goaledCars[CAR_NUM] = { nullptr };	//�S�[���ς݂̎ԁB
	int m_goaledCarsNum = 0;					//�S�[���ς݂̎Ԃ̐��B
	LapUI* m_LapUI = nullptr;					//���񐔂̕\�������s���N���X�B
	ReverseRunUI* m_reverseRunUI = nullptr;		//�t�����̕\�����s���N���X�B
	GoalUI* m_goalUI = nullptr;					//�S�[���\�����s���N���X�B
	int m_GoaledCarNum = 0;						//�S�[���ς݂̎Ԃ̐��B
};

