#pragma once

//機能の試運転用クラス。

class CGameCamera;
class Player;
class Car;
class Spectator;

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//初期化。
	void Release();		//解放処理が必要ならこれ。
	void PreUpdate()override;
	void Update();		//更新処理。
	void PreRender(RenderContext& rc) override;		//描画処理。

private:
	CLevel m_level;
	CGameCamera* m_camera = nullptr;
	Player* m_player = nullptr;
	Car* m_car = nullptr;
	Spectator* m_spectator = nullptr;
	prefab::ModelRender* m_testBox = nullptr;			//テスト用の箱。
	ModelInitData initData;
	std::vector<prefab::CDirectionLight*> m_lightArray;
	Vector3 m_lightDir = Vector3::Zero;
	float m_lightPow = 0.5f;
};

