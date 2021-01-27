#pragma once

//�@�\�̎��^�]�p�N���X�B

#include "physics\PhysicsStaticObject.h"

class CGameCamera;
class Car;
class Spectator;
class SceneLight;

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//�������B
	void Release();		//����������K�v�Ȃ炱��B
	void PreUpdate()override;
	void Update();		//�X�V�����B

private:
	CLevel m_level;
	CGameCamera* m_camera = nullptr;
	Car* m_car = nullptr;
	Spectator* m_spectator = nullptr;
	SceneLight* m_light = nullptr;
	float m_lightPow = 0.5f;
};

