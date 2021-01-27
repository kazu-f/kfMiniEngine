#pragma once

//機能の試運転用クラス。

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
	bool Start();		//初期化。
	void Release();		//解放処理が必要ならこれ。
	void PreUpdate()override;
	void Update();		//更新処理。

private:
	CLevel m_level;
	CGameCamera* m_camera = nullptr;
	Car* m_car = nullptr;
	Spectator* m_spectator = nullptr;
	SceneLight* m_light = nullptr;
	float m_lightPow = 0.5f;
};

