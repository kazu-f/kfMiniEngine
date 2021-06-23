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
	EnChoises m_currentChoises = en_GameStart;								//現在選択中の選択肢。
	std::unique_ptr<ITitleChoises> m_titleChoises[en_ChoisesNum];		//タイトル選択肢。
	prefab::CSpriteRender* m_titleSprite = nullptr;							//タイトルのスプライト。
	PickArrow* m_pickArrow = nullptr;										//現在選択中のものを指す矢印。
	bool m_isDecision = false;												//決定をしたか？
};

