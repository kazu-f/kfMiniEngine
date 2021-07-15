#pragma once
#include "CarMoveController.h"
class CheckedController;
class ICarDriver;
class SpeedoMeter;
class CarSoundController;

namespace CAR{
	enum EnCarColor {
		enCar_Red,		//赤
		enCar_Blue,		//青
		enCar_Num		//種類数
	};
}

class Car : public IGameObject
{
public:
	enum EnDriverType {
		enTypePlayer,			//プレイヤー操作。
		enTypeAI,				//AI操作。
		enDriverTypeNum
	};

public:
	Car();
	~Car();
	bool Start() override;			//初期化処理。
	void Update() override;			//更新処理。
	void OnDestroy() override;		//削除時処理。
public:		//Set関数とか
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	void SetCarColor(const CAR::EnCarColor colorType)
	{
		m_carColor = colorType;
	}
	//車のドライバを設定する。
	void SetCarDriver(EnDriverType type);

	void SetIsPlayer(const bool isPlayer)
	{
		m_isPlayer = isPlayer;
	}
	//カウントダウン中かを指定。
	void SetIsRaceCountDown(const bool isCountDown)
	{
		m_isRaceCountDown = isCountDown;
	}
	//レースが開始されたかを指定。
	void SetIsRaceStart(const bool isRaceStart)
	{
		m_isRaceStart = isRaceStart;
	}
public:		//Get関数とか
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	//移動コントローラーを取得。
	CarMoveController* GetCarMoveController()
	{
		return m_carMoveCon.get();
	}
	//チェックコントローラーを取得。
	CheckedController* GetCheckedController()
	{
		return m_checkedCon.get();
	}
	//ゴール済みか？
	bool IsGoal();
private:
	enum EnCarStartStep {
		en_initCar,
		en_waitCountDown,
		en_waitRaceStart,
		en_carStartStep
	};
	EnCarStartStep m_startStep = en_initCar;

private:
	CAR::EnCarColor m_carColor = CAR::enCar_Red;
	prefab::ModelRender* m_model = nullptr;

	std::unique_ptr<ICarDriver> m_carDriver;					//車の操作のドライバー。
	std::unique_ptr<CarMoveController> m_carMoveCon;			//車の移動を管理する。
	std::unique_ptr<CheckedController> m_checkedCon;			//チェックポイントに監視される。
	std::unique_ptr<CarSoundController> m_soundCon;			//チェックポイントに監視される。
	std::unique_ptr<SpeedoMeter> m_speedoMeter;					//スピードメーターのスプライト。

	Vector3 m_position = Vector3::Zero;				//座標。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
	bool m_isPlayer = false;						//プレイヤーか？
	bool m_isRaceStart = false;						//レースが開始されたか。
	bool m_isRaceCountDown = false;						//カウントダウンが開始されたか。
};

