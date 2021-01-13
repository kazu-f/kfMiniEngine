#pragma once

//�@�\�̎��^�]�p�N���X�B

class CGameCamera;
class Player;
class Car;
class Spectator;

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//�������B
	void Release();		//����������K�v�Ȃ炱��B
	void PreUpdate()override;
	void Update();		//�X�V�����B
	void PreRender(RenderContext& rc) override;		//�`�揈���B

private:
	CLevel m_level;
	CGameCamera* m_camera = nullptr;
	Player* m_player = nullptr;
	Car* m_car = nullptr;
	Spectator* m_spectator = nullptr;
	prefab::ModelRender* m_testBox = nullptr;			//�e�X�g�p�̔��B
	ModelInitData initData;
	std::vector<prefab::CDirectionLight*> m_lightArray;
	Vector3 m_lightDir = Vector3::Zero;
	float m_lightPow = 0.5f;
};

