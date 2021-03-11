#pragma once

class ICarState;
class CarStateIdle;
class CarStateAccele;
class CarStateBrake;
class CheckedController;

class Car : public IGameObject
{
public:
	Car();
	~Car();
	bool Start() override;			//初期化処理。
	void Update() override;			//更新処理。
	void OnDestroy() override;		//削除時処理。

	friend class ICarState;
	friend class CarStateAccele;
	friend class CarStateIdle;
	friend class CarStateBrake;
public:
	void ChangeState(ICarState* state);
public:		//Set関数とか
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

public:		//Get関数とか
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	const Vector3& GetForward()const
	{
		return m_forward;
	}

	float GetSpeed()const 
	{
		return m_speed;
	}
	//チェックコントローラーを取得。
	CheckedController* GetCheckedController()
	{
		return m_checkedCon.get();
	}

public:		//車の移動などの処理
	/// <summary>
	/// 加速度を速度に加算する。
	/// </summary>
	/// <param name="accele">加速度。</param>
	void AddAcceleration(const float accele)
	{
		m_speed = min(MAX_SPEED, m_speed + (accele * 60.0f * GameTime().GetFrameDeltaTime()));
	}
	/// <summary>
	/// 減速度を与える。
	/// </summary>
	/// <param name="diccele">減速度。</param>
	void AddDicceleration(const float diccele)
	{
		m_speed = max(0.0f, m_speed - (diccele * 60.0f * GameTime().GetFrameDeltaTime()));
	}
	/// <summary>
	/// 車の向きを計算する。
	/// </summary>
	void CalcDirection();

protected:
	static constexpr float MAX_SPEED = 6800.0f;
protected:
	ICarState* m_currentState = nullptr;
	std::unique_ptr<CarStateIdle> m_stateIdle;
	std::unique_ptr<CarStateAccele> m_stateAccele;
	std::unique_ptr<CarStateBrake> m_stateBrake;
protected:
	prefab::ModelRender* m_model = nullptr;
	//移動関係。
	CCharacterController m_charaCon;		//キャラコン。
	std::unique_ptr<CheckedController> m_checkedCon;			//チェックポイントに監視される。
	Vector3 m_position = Vector3::Zero;		//座標。
	Vector3 m_moveSpeed = Vector3::Zero;	//移動速度。
	float m_speed = 0.0f;					//速度。
	//車の向き
	Vector3 m_forward = Vector3::AxisZ;		//車の前方向。
	Vector3 m_right = Vector3::AxisX;		//車の右方向。
	Vector3 m_up = Vector3::AxisY;			//車の上方向。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
};

