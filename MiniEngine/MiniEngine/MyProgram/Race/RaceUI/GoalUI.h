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

	bool IsTimePassed()const
	{
		return m_currentElapsedTime > TIME_LIMIT;
	}

	//ゴール表示を有効にする。
	void ActiveGoal()
	{
		this->SetActiveFlag(true);
		m_sprite->SetActiveFlag(true);
	}

private:
	prefab::CSpriteRender* m_sprite = nullptr;			//スプライト。
	float m_currentElapsedTime = 0.0f;					//経過時間。
	static const float TIME_LIMIT;						//経過時間のリミット。
};

