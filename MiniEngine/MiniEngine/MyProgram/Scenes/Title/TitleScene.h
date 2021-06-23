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
	prefab::CSpriteRender* m_titleSprite = nullptr;							//�^�C�g���̃X�v���C�g�B
	prefab::CSpriteRender* m_choisesSprite[en_ChoisesNum] = { nullptr };	//�I�����̃X�v���C�g�B
	prefab::CSpriteRender* m_pickSprite = nullptr;							//�s�b�N���̕����w���X�v���C�g�B

};

