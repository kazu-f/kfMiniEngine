#pragma once
class TitleScene : public IGameObject
{
public:
	TitleScene();
	~TitleScene();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;

private:
	enum EnChoises {
		en_GameStart,
		en_ChoisesNum
	};

private:
	prefab::CSpriteRender* m_titleSprite = nullptr;							//タイトルのスプライト。
	prefab::CSpriteRender* m_choisesSprite[en_ChoisesNum] = { nullptr };	//選択肢のスプライト。
	prefab::CSpriteRender* m_pickSprite = nullptr;							//ピック中の物を指すスプライト。

};

