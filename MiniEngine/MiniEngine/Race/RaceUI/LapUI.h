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

public:		//Set�֐��B
	/// <summary>
	/// ���݂̎��񐔂��Z�b�g�B
	/// </summary>
	/// <param name="num">���񐔁B</param>
	void SetCurrentLap(int num)
	{
		m_nextLapNum = max(m_nextLapNum,num);
	}

public:		//Get�֐��B
	/// <summary>
	/// �S�[���������H
	/// </summary>
	bool IsGoal() const
	{
		return m_isGoal;
	}

private:	//���������B
	/// <summary>
	/// ���񐔂����������̏����B
	/// </summary>
	void CountUpLap();

private:
	prefab::CSpriteRender* m_currentLapSprite[enLap_Num] = { nullptr };	//���݂̎��񐔁B
	prefab::CSpriteRender* m_maxLapSprite = nullptr;					//����̍ő吔�B
	prefab::CSpriteRender* m_slashSprite = nullptr;						//�X���b�V���B(/)
	prefab::CSpriteRender* m_lapTextSprite = nullptr;					//�e�L�X�g�B(Lap)

	int m_currentLapNum = 0;								//���݂̎��񐔁B
	int m_nextLapNum = 0;									//���̎��񐔁B
	float m_currentTime = 0.0f;								//�_�Ŏ��ԁB
	bool m_isGoal = false;									//�S�[���������H
};

