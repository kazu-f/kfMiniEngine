#pragma once

class Car;
class ICarState;
class CarStateIdle;
class CarStateAccele;
class CarStateBrake;
class CarStateDrift;
class ICarDriver;

class CarMoveController
{
public:
	static const float MOVE_COEFFICIENT;				//時間ベースの移動に掛ける係数。

public:

	enum EnCarState {
		enStateIdle,
		enStateAccele,
		enStateBrake,
		enStateDrift,
		enStateNum
	};

public:
	CarMoveController();
	~CarMoveController();
	/// <summary>
	/// キャラコンとかの初期化はインスタンス生成より後に行いたい。
	/// </summary>
	void Init(const Vector3& position, const Quaternion& rotation);
	/// <summary>
	/// 車の移動処理。
	/// </summary>
	/// <returns>移動可能かどうかの判定。</returns>
	bool MoveCar();
	/// <summary>
	/// ステートを変更する。
	/// </summary>
	/// <param name="state">列挙で指定を行う。</param>
	void ChangeState(EnCarState state);

public:		//車の移動などの処理
	/// <summary>
	/// 減速度を与える。
	/// </summary>
	/// <param name="diccele">減速度。</param>
	void AddDicceleration(const float diccele);
	/// <summary>
	/// 車の向きを計算する。
	/// </summary>
	void CalcDirection();
	/// <summary>
	/// 加速度を速度に加算する。
	/// </summary>
	/// <param name="accele">加速度。</param>
	void AddAcceleration(const float accele)
	{
		m_speed = min(MAX_SPEED, m_speed + (accele * MOVE_COEFFICIENT * GameTime().GetFrameDeltaTime()));
	}
public:		//Set関数。
	/// <summary>
	/// 車のドライバを設定する。
	/// </summary>
	/// <param name="driver">ドライバのアドレス。</param>
	void SetCarDriver(ICarDriver* driver)
	{
		m_carDriver = driver;
	}
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

public:		//Get関数。
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	const Vector3& GetForward()const
	{
		return m_forward;
	}
	float GetSpeed()const
	{
		return m_speed;
	}
	CCharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
	const ICarDriver* GetCarDriver()const
	{
		return m_carDriver;
	}

public:
	static constexpr float MAX_SPEED = 6800.0f;

private:	//ステート等
	ICarState* m_currentState = nullptr;
	std::unique_ptr<ICarState> m_carStates[EnCarState::enStateNum];
	//車の操作のドライバ。
	ICarDriver* m_carDriver = nullptr;
	//移動関係。
	CCharacterController m_charaCon;		//キャラコン。
	Vector3 m_position = Vector3::Zero;		//座標。
	//車の向き
	Vector3 m_forward = Vector3::AxisZ;		//車の前方向。
	Vector3 m_right = Vector3::AxisX;		//車の右方向。
	Vector3 m_up = Vector3::AxisY;			//車の上方向。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
	Vector3 m_moveSpeed = Vector3::Zero;	//移動速度。
	float m_speed = 0.0f;					//速度。
};

