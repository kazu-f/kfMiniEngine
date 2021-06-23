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
	PickArrow* m_pickArrow = nullptr;										//���ݑI�𒆂̂��̂��w�����B
	bool m_isDecision = false;												//������������H
};

