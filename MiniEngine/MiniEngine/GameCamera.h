#pragma once

class CGameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

private:
	float m_targetToPosLen = 0.0f;
};

