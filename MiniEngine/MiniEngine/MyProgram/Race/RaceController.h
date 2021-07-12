#pragma once
#include "RaceEnum.h"
class Car;
class LapUI;
class ReverseRunUI;
class GoalUI;
class RaceStartCountDown;

/// <summary>
/// レースを管理する。
/// </summary>
class RaceController : public IGameObject
{
public:
	RaceController();
	~RaceController();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	//プレイヤーを指定する。
	void SetPlayer(Car* player)
	{
		m_player = player;
	}
	//CPUを登録する。
	void RegistCPU(Car* cpus)
	{
		ENGINE_ASSERT(
			m_cpusNum < MAX_CPU_NUM,
			"CPUの最大数を超えています。"
		);
		m_cpus[m_cpusNum] = cpus;
		m_cpusNum++;
	}
	//レースシーンに移行できたか。
	void SetInRaceScene(bool flag)
	{
		m_isInRaceScene = flag;
	}
	//レースが終了したか？
	bool IsEndRace();

private:
	/// <summary>
	/// ゴール済みの車を更新する。
	/// </summary>
	void GoaledCarUpdate();
	/// <summary>
	/// ゴール済みの車を登録。
	/// </summary>
	/// <param name="goaledCar">ゴール済みの車。</param>
	void RegistGoaledCar(Car* goaledCar);

public:
	static const int MAX_CPU_NUM = 5;
	static const int CAR_NUM = 6;

private:
	Car* m_player = nullptr;		//プレイヤー。
	Car* m_cpus[MAX_CPU_NUM] = { nullptr };		//CPU。
	int m_cpusNum = 0;					//CPUの数。
	Car* m_goaledCars[CAR_NUM] = { nullptr };	//ゴール済みの車。
	int m_goaledCarsNum = 0;					//ゴール済みの車の数。
	RaceStartCountDown* m_raceStart = nullptr;	//レース開始のカウントダウンを行うクラス。
	LapUI* m_LapUI = nullptr;					//周回数の表示等を行うクラス。
	ReverseRunUI* m_reverseRunUI = nullptr;		//逆走中の表示を行うクラス。
	GoalUI* m_goalUI = nullptr;					//ゴール表示を行うクラス。
	int m_GoaledCarNum = 0;						//ゴール済みの車の数。
	bool m_isInRaceScene = false;				//レースシーンに移行完了できたか。
};

