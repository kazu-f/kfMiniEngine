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
	//�S�[�����Ă���̌o�ߎ��Ԃ����ȏ�B
	bool IsTimePassed()const
	{
		return m_currentElapsedTime > TIME_LIMIT;
	}

	/// <summary>
	/// �S�[���\����L���ɂ���B
	/// </summary>
	/// <param name="winFlag">�����������ǂ����H</param>
	void ActiveGoal(bool winFlag)
	{
		this->SetActiveFlag(true);
		m_sprite->SetActiveFlag(true);
		m_win->SetActiveFlag(winFlag);
		m_lose->SetActiveFlag(!winFlag);
	}

private:
	prefab::CSpriteRender* m_sprite = nullptr;			//�X�v���C�g�B
	prefab::CSpriteRender* m_win = nullptr;				//�����\�L�B
	prefab::CSpriteRender* m_lose = nullptr;			//�s�k�\�L�B
	float m_currentElapsedTime = 0.0f;					//�o�ߎ��ԁB
	static const float TIME_LIMIT;						//�o�ߎ��Ԃ̃��~�b�g�B
};

