#pragma once
/// <summary>
/// 機能の試運転を行うクラス。
/// </summary>

class Player;
class PlayerBall;
class SceneLight;
class CGameCamera;
class BackGround;

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
	void InitSprite();

private:
	CLevel m_courseLevel;
	Player* m_player = nullptr;
	PlayerBall* m_ball = nullptr;
	prefab::CSoundSource* m_bgm = nullptr;
	prefab::CSoundSource* m_se = nullptr;
	prefab::CSpriteRender* m_sprite = nullptr;
	CGameCamera* m_camera = nullptr;
	SceneLight* m_light = nullptr;
	BackGround* m_bg = nullptr;
};

