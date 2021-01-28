#pragma once

class CGameCamera;

class Car : public IGameObject
{
public:
	Car() {};
	~Car() {};
	bool Start() override;			//初期化処理。
	void Update() override;			//更新処理。
	void OnDestroy() override;		//削除時処理。

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

private:
	CGameCamera* m_camera = nullptr;
	prefab::ModelRender* m_model = nullptr;

	CCharacterController m_charaCon;		//キャラコン。
	Vector3 m_position = Vector3::Zero;		//座標。
	Vector3 m_moveSpeed = Vector3::Zero;	//移動速度。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
};

