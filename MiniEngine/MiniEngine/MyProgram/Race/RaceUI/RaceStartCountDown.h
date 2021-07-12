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
	bool Start() override final;		//開始処理。
	void OnDestroy() override final;	//削除時処理。
	void Update() override final;		//更新処理。
	//レースがスタートしているか。
	bool IsRaceStarted()
	{
		return m_countState >= en_start;
	}
	
private:
	//カウントを行う。(フェード等も行う。)
	void CountFade(const float time);
	//カウントを開始するまで少し時間を開ける。
	bool WaitCountEnable(const float time);
	//カウントのサウンドを流す。
	void CountSound();

private:
	prefab::CSpriteRender* m_countDownSprite[enNum] = { nullptr };		//カウントのスプライト。
	prefab::CSoundSource* m_seCount[enNum] = { nullptr };				//カウントのSE。
	int m_countState = 0;						//現在のカウントを表すステート変数。
	float m_timeIsWait = 0.0f;					//カウント開始前の経過時間。
	float m_timeInSecond = 0.0f;				//カウント開始してからの経過時間。
	bool m_countUpNow = true;					//カウントが変わったフラグ。
};

