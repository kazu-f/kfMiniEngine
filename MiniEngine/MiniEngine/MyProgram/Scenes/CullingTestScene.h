#pragma once
class Spectator;
class CGameCamera;

//カリングのテストを行う。
class CullingTestScene : public IGameObject
{
public:
	CullingTestScene();
	~CullingTestScene();
	bool Start();
	void OnDestroy();
	void Update();

private:
	Spectator* m_spectator = nullptr;		//インスタンシング描画を行うクラス。
	CGameCamera* m_camera = nullptr;			//カメラ。
	CLevel m_level;							//レベル読み込み。
	prefab::CDirectionLight* m_light = nullptr;
};

