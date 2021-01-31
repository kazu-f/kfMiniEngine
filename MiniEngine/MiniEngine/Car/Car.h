#pragma once

class CGameCamera;
class ICarState;
class CarStateIdle;
class CarStateAccele;

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
public:		//Set関数とか
	void SetCamera(CGameCamera* camera)
	{
		m_camera = camera;
	}
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

public:		//車の移動などの処理
	/// <summary>
	/// 加速度を速度に加算する。
	/// </summary>
	/// <param name="accele">加速度。</param>
	void AddAcceleration(const Vector3& accele)
	{
		m_moveSpeed += accele;
	}
	/// <summary>
	/// 車の向きを計算する。
	/// </summary>
	void CalcDirection();

private:
	CGameCamera* m_camera = nullptr;
protected:
	ICarState* m_currentState = nullptr;
	CarStateIdle m_stateIdle;
	CarStateAccele m_stateAccele;
protected:
	prefab::ModelRender* m_model = nullptr;
	CCharacterController m_charaCon;		//キャラコン。
	Vector3 m_position = Vector3::Zero;		//座標。
	Vector3 m_moveSpeed = Vector3::Zero;	//移動速度。
	Vector3 m_forward = Vector3::AxisZ;		//車の前方向。
	Vector3 m_right = Vector3::AxisX;		//車の右方向。
	Vector3 m_up = Vector3::AxisY;			//車の上方向。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
};

