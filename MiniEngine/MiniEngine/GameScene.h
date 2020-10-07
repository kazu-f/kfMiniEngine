#pragma once

//機能の試運転用クラス。

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//初期化。
	void Release();		//解放処理が必要ならこれ。
	void Update();		//更新処理。
	void ForwardRender(RenderContext& rc) override;		//描画処理。

private:
	//Light light;		//ライト。
	Model	robotModel;			//モデル。
	ModelInitData initData;
	Skeleton m_skeleton;
	CAnimation m_animation;
	std::vector<CAnimationClipPtr> m_animationClips;	//アニメーションクリップの配列。
	std::vector<prefab::CDirectionLight*> m_lightArray;
	Vector3 m_lightDir = Vector3::Zero;
	bool isPBR = true;
};

