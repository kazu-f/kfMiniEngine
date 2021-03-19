#pragma once

/// <summary>
/// �S�[���\�L�B
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

	//�S�[���\����L���ɂ���B
	void ActiveGoal()
	{
		this->SetActiveFlag(true);
		m_sprite->SetActiveFlag(true);
	}

private:
	prefab::CSpriteRender* m_sprite = nullptr;			//�X�v���C�g�B
	float m_currentElapsedTime = 0.0f;					//�o�ߎ��ԁB
	static const float TIME_LIMIT;						//�o�ߎ��Ԃ̃��~�b�g�B
};

