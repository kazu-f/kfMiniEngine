#pragma once

#include "characterController\CharacterController.h"

class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};
	bool Start() override;
	void Update() override;
	void OnDestroy() override;

public:		//Setä÷êîÇ∆Ç©
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}


private:
	enum EnAnimState
	{
		en_animIdle,
		en_animRun,
		en_animNum
	};

	CCharacterController m_chara;
	prefab::ModelRender* m_model = nullptr;

	Vector3 m_position = Vector3::Zero;		//ç¿ïWÅB
};

