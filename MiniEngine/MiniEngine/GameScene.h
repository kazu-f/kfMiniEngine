#pragma once

//機能の試運転用クラス。

class CGameCamera;

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
	CGameCamera* m_camera = nullptr;
	//Light light;		//ライト。
	Model	m_model;			//モデル。
	Model	m_testBox;			//テスト用の箱。
	ModelInitData initData;
	Skeleton m_skeleton;
	CAnimation m_animation;
	std::vector<CAnimationClipPtr> m_animationClips;	//アニメーションクリップの配列。
	std::vector<prefab::CDirectionLight*> m_lightArray;
	Vector3 m_lightDir = Vector3::Zero;
	float m_lightPow = 0.5f;
	bool isPBR = true;
};

