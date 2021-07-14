#pragma once

class CGameCamera : public IGameObject
{
public:
	bool Start()override final;		//初期化処理。
	void Update()override final;	//更新処理。
public:
	/// <summary>
	/// ターゲットの座標を設定。
	/// </summary>
	void SetTarget(const Vector3& target)
	{
		m_target = target;
	}
private:
	void MoveTarget();

private:
	Vector3 m_target = Vector3::Zero;		//ターゲットの座標。
	float m_targetToPosLen = 0.0f;			//ターゲットまでの距離。
};

