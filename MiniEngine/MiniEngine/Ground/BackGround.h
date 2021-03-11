#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	/// <summary>
	/// 座標、回転、スケールを設定する。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	/// <param name="scale">スケール。</param>
	void InitData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		m_position = pos;
		m_rotation = rot;
		m_scale = scale;
	}

private:
	prefab::ModelRender* m_model = nullptr;		//モデル表示。
	TkmFile m_mesh;								//当たり判定用のメッシュ。
	CPhysicsStaticObject m_phyStaticObj;		//静的物理オブジェクト。
	Vector3 m_position = Vector3::Zero;			//座標。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
	Vector3 m_scale = Vector3::One;				//スケール。
};

