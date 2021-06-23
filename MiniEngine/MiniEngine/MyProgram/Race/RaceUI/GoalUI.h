#pragma once

/// <summary>
/// ゴール表記。
/// </summary>
class GoalUI : public IGameObject
{
public:
	GoalUI();
	~GoalUI();

	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	//ゴールしてからの経過時間が一定以上。
	bool IsTimePassed()const
	{
		return m_currentElapsedTime > TIME_LIMIT;
	}

	/// <summary>
	/// ゴール表示を有効にする。
	/// </summary>
	/// <param name="winFlag">勝利したかどうか？</param>
	void ActiveGoal(bool winFlag)
	{
		this->SetActiveFlag(true);
		m_sprite->SetActiveFlag(true);
		m_win->SetActiveFlag(winFlag);
		m_lose->SetActiveFlag(!winFlag);
	}

private:
	prefab::CSpriteRender* m_sprite = nullptr;			//スプライト。
	prefab::CSpriteRender* m_win = nullptr;				//勝利表記。
	prefab::CSpriteRender* m_lose = nullptr;			//敗北表記。
	float m_currentElapsedTime = 0.0f;					//経過時間。
	static const float TIME_LIMIT;						//経過時間のリミット。
};

