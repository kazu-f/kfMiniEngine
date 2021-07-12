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
	bool Start() override final;		//�J�n�����B
	void OnDestroy() override final;	//�폜�������B
	void Update() override final;		//�X�V�����B
	//���[�X���X�^�[�g���Ă��邩�B
	bool IsRaceStarted()
	{
		return m_countState >= en_start;
	}
	
private:
	//�J�E���g���s���B(�t�F�[�h�����s���B)
	void CountFade(const float time);
	//�J�E���g���J�n����܂ŏ������Ԃ��J����B
	bool WaitCountEnable(const float time);
	//�J�E���g�̃T�E���h�𗬂��B
	void CountSound();

private:
	prefab::CSpriteRender* m_countDownSprite[enNum] = { nullptr };		//�J�E���g�̃X�v���C�g�B
	prefab::CSoundSource* m_seCount[enNum] = { nullptr };				//�J�E���g��SE�B
	int m_countState = 0;						//���݂̃J�E���g��\���X�e�[�g�ϐ��B
	float m_timeIsWait = 0.0f;					//�J�E���g�J�n�O�̌o�ߎ��ԁB
	float m_timeInSecond = 0.0f;				//�J�E���g�J�n���Ă���̌o�ߎ��ԁB
	bool m_countUpNow = true;					//�J�E���g���ς�����t���O�B
};

