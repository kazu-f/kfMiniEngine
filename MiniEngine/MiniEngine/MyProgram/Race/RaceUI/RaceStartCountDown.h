#pragma once
class RaceStartCountDown : public IGameObject
{
public:
	enum EnCountDown {
		en_three,
		en_two,
		en_one,
		en_start,
		enNum
	};

public:
	RaceStartCountDown();
	~RaceStartCountDown();
	bool Start() override final;
	void OnDestroy() override final;
	void Update() override final;
	//レースがスタートしているか。
	bool IsRaceStarted()
	{
		return m_countState >= en_start;
	}
	
private:
	void CountFade(const float time);
	bool WaitCountEnable(const float time);
	void CountSound();

private:
	prefab::CSpriteRender* m_countDownSprite[enNum] = { nullptr };
	prefab::CSoundSource* m_seCount[enNum] = { nullptr };
	int m_countState = 0;
	float m_timeIsWait = 0.0f;
	float m_timeInSecond = 0.0f;
	bool m_countUpNow = true;
};

