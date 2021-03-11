#pragma once
/// <summary>
/// ゲームクラス。
/// </summary>

class FollowCarCamera;
class Car;
class Spectator;
class Guardrail;
class SceneLight;
class CheckPointManager;

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
	//初期化ステップ。
	enum EnInitStep {
		enInit_Other,
		enInit_Course,
		enInit_Spectator,
		enInit_CheckPoint,
	};
	//観客の列挙。
	enum EnSpectator {
		enShirtMale,
		enNathanMale,
		enSophiaWoman,
		enClaudiaWoman,
		enSpectatorNum
	};
	EnInitStep m_initState = enInit_Other;	//初期化ステップ。

	CLevel m_courseLevel;					//コースのレベル。
	CLevel m_spectatorLevel;				//観客のレベル。
	CheckPointManager* m_checkPointManager = nullptr;	//チェックポイントの管理者。
	FollowCarCamera* m_camera = nullptr;		//カメラ。
	Car* m_car = nullptr;					//車。
	Spectator* m_spectator[enSpectatorNum] = { nullptr };		//観客。
	Guardrail* m_guardrail = nullptr;		//ガードレール。
	SceneLight* m_light = nullptr;			//照明。
};

