#pragma once
#include "physics/SphereCollider.h"
#include "physics/BoxCollider.h"

/// <summary>
/// 物理エンジンを試す。
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
	CSphereCollider m_sphere;		//コライダー。
	CBoxCollider m_box;				//コライダー。
	CRigidBody m_rigidBody;			//剛体。
};

