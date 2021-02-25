#pragma once
/// <summary>
/// 機能の試運転を行うクラス。
/// </summary>

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
	void OnBGM();
	void PlaySE();

private:
	CLevel m_courseLevel;
	Player* m_player = nullptr;
	prefab::CSoundSource* m_bgm = nullptr;
	prefab::CSoundSource* m_se = nullptr;
	CGameCamera* m_camera = nullptr;
	SceneLight* m_light = nullptr;
};

