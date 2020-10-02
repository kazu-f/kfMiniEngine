#pragma once

//機能の試運転用クラス。

class GameScene
{
public:
	GameScene();
	~GameScene();
	void Init();		//初期化。
	void Release();		//解放処理が必要ならこれ。
	void Update();		//更新処理。
	void Draw(RenderContext& rc);		//描画処理。

private:
	Light light;		//ライト。
	Model robotPBRModel;		//モデル。
	Model	robotModel;			//モデル。
	ModelInitData initData;
	Skeleton m_skeleton;
	CAnimation m_animation;
	std::vector<CAnimationClipPtr> m_animationClips;	//アニメーションクリップの配列。
	bool isPBR = true;
};

