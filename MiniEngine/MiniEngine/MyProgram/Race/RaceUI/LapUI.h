#pragma once

class LapUI : public IGameObject
{
public:
	enum EnLapNum {
		enLap_One,
		enLap_Two,
		enLap_Three,
		enLap_Num
	};

	LapUI();
	~LapUI();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;

public:		//Set関数。
	/// <summary>
	/// 現在の周回数をセット。
	/// </summary>
	/// <param name="num">周回数。</param>
	void SetCurrentLap(int num)
	{
		m_nextLapNum = max(m_nextLapNum,num);
	}

public:		//Get関数。
	/// <summary>
	/// ゴールしたか？
	/// </summary>
	bool IsGoal() const
	{
		return m_isGoal;
	}

private:	//内部処理。
	/// <summary>
	/// 周回数が増えた時の処理。
	/// </summary>
	void CountUpLap();

private:
	prefab::CSpriteRender* m_currentLapSprite[enLap_Num] = { nullptr };	//現在の周回数。
	prefab::CSpriteRender* m_maxLapSprite = nullptr;					//周回の最大数。
	prefab::CSpriteRender* m_slashSprite = nullptr;						//スラッシュ。(/)
	prefab::CSpriteRender* m_lapTextSprite = nullptr;					//テキスト。(Lap)

	int m_currentLapNum = 0;								//現在の周回数。
	int m_nextLapNum = 0;									//次の周回数。
	float m_currentTime = 0.0f;								//点滅時間。
	bool m_isGoal = false;									//ゴールしたか？
};

