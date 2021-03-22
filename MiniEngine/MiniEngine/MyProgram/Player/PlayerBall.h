#pragma once
#include "physics/SphereCollider.h"
#include "physics/BoxCollider.h"

/// <summary>
/// �����G���W���������B
/// </summary>
class PlayerBall : public IGameObject
{
public:
	PlayerBall();
	~PlayerBall();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;

private:
	prefab::ModelRender* m_model = nullptr;
	CSphereCollider m_sphere;		//�R���C�_�[�B
	CBoxCollider m_box;				//�R���C�_�[�B
	CRigidBody m_rigidBody;			//���́B
};

