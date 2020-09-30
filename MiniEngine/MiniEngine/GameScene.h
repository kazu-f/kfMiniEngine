#pragma once

//�@�\�̎��^�]�p�N���X�B

class GameScene
{
public:
	GameScene();
	~GameScene();
	void Init();		//�������B
	void Release();		//����������K�v�Ȃ炱��B
	void Update();		//�X�V�����B
	void Draw(RenderContext& rc);		//�`�揈���B

private:
	Light light;		//���C�g�B
	Model robotPBRModel;		//���f���B
	Model	robotModel;			//���f���B
	ModelInitData initData;
	bool isPBR = true;
};

