#pragma once
/// <summary>
/// ゲームクラス。
/// </summary>
#include "Spectator/SpectatorNames.h"

class FollowCarCamera;
class Car;
class Spectator;
class Guardrail;
class SceneLight;
class CheckPointManager;
class BackGround;
class RaceController;

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//初期化。
	void Release();		//解放処理が必要ならこれ。
	void PreUpdate()override;
	void Update();		//更新処理。
	void OnDestroy()override final;	//削除時処理。

private:	//初期化ステップで呼び出す処理。
	void InitOther();			//カメラやライトを初期化。
	void InitCourse();			//コースを読み込む。
	void InitSpectator();		//観客を配置する。
	void InitRaceController();	//レースの制御を行うクラスの初期化。
	void InitCheckPoint();		//チェックポイントを配置する。

private:
	/// <summary>
	/// 観客のデータを追加していく。
	/// </summary>
	/// <param name="objData">オブジェクトのデータ。</param>
	/// <param name="type">観客の種類</param>
	void AddSpectatorData(SLevelObjectData& objData,int type);

private:
	//初期化ステップ。
	enum EnInitStep {
		enInit_Other,				//ライトやカメラの作成。
		enInit_Course,				//コースの読み込み。
		enInit_Spectator,			//観客の配置。
		enInit_RaceController,		//レース制御クラスの作成。
		enInit_CheckPoint,			//チェックポイントの配置。
		enInit_End,					//初期化ステップ終了。
	};
	EnInitStep m_initState = enInit_Other;	//初期化ステップ。

	CLevel m_courseLevel;					//コースのレベル。
	CLevel m_spectatorLevel;				//観客のレベル。
	CheckPointManager* m_checkPointManager = nullptr;	//チェックポイントの管理者。
	RaceController* m_raceController = nullptr;			//レースの管理。
	FollowCarCamera* m_camera = nullptr;		//カメラ。
	Car* m_playerCar = nullptr;					//車。
	Car* m_cpuCar = nullptr;					//車。
	BackGround* m_ground = nullptr;			//地面。
	Spectator* m_spectator[enSpectatorNum] = { nullptr };		//観客。
	Guardrail* m_guardrail = nullptr;		//ガードレール。
	SceneLight* m_light = nullptr;			//照明。
	prefab::CSky* m_sky = nullptr;			//空。
};

