#pragma once

class CGameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;
public:
	void SetTarget(const Vector3& target)
	{
		m_target = target;
	}

private:
	Vector3 m_target = Vector3::Zero;
	float m_targetToPosLen = 0.0f;
};

