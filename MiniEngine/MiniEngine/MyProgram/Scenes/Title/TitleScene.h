#pragma once

class PickArrow;
class ITitleChoises;
class ChoiseGameStart;

class TitleScene : public IGameObject
{
public:
	enum EnChoises {
		en_GameStart,
		en_ChoisesNum
	};

public:
	TitleScene();
	~TitleScene();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;

private:

private:
	EnChoises m_currentChoises = en_GameStart;								//���ݑI�𒆂̑I�����B
	std::unique_ptr<ITitleChoises> m_titleChoises[en_ChoisesNum];		//�^�C�g���I�����B
	prefab::CSpriteRender* m_titleSprite = nullptr;							//�^�C�g���̃X�v���C�g�B
	prefab::CSpriteRender* m_pressA = nullptr;								//PressA�\���B
	prefab::CSoundSource* m_titleBGM = nullptr;								//�^�C�g����BGM�B
	PickArrow* m_pickArrow = nullptr;										//���ݑI�𒆂̂��̂��w�����B
	float m_currentTime = 0.0f;												//�^�C�g���o�ߎ��ԁB
	bool m_isDecision = false;												//������������H
};

