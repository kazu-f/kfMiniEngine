#pragma once
class CheckedController;
class LapUI;
class ReverseRunUI;

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

private:
	CheckedController* m_player = nullptr;		//�v���C���[�B
	LapUI* m_LapUI = nullptr;					//���񐔂̕\�������s���N���X�B
	ReverseRunUI* m_reverseRunUI = nullptr;		//�t�����̕\�����s���N���X�B
};

