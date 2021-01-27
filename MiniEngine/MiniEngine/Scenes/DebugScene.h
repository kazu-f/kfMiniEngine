#pragma once

class Player;
class SceneLight;
class CGameCamera;

class DebugScene:public IGameObject
{
public:
	DebugScene();
	~DebugScene();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;

private:
	CLevel m_courseLevel;
	Player* m_player = nullptr;
	CGameCamera* m_camera = nullptr;
	SceneLight* m_light = nullptr;
};

