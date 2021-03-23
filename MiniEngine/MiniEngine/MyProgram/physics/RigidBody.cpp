#include "stdafx.h"
#include "RigidBody.h"

namespace Engine {
	CRigidBody::~CRigidBody()
	{
		Release();
	}
	void CRigidBody::Release()
	{
		if (m_rigidBody) {
			PhysicsWorld().RemoveRigidBody(*this);
		}
	}
	void CRigidBody::Create(RigidBodyInfo& rbInfo, bool isRegistPhysicsWorld)
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(rbInfo.pos.x, rbInfo.pos.y, rbInfo.pos.z));
		transform.setRotation(btQuaternion(rbInfo.rot.x, rbInfo.rot.y, rbInfo.rot.z, rbInfo.rot.w));
		m_myMotionState = std::make_unique<btDefaultMotionState>();
		m_myMotionState->setWorldTransform(transform);
		btVector3 btLocalInteria;
		rbInfo.localInteria.CopyTo(btLocalInteria);
		btRigidBody::btRigidBodyConstructionInfo btRbInfo(rbInfo.mass, m_myMotionState.get(), rbInfo.collider->GetBody(), btLocalInteria);
		//剛体を作成。
		m_rigidBody = std::make_unique<btRigidBody>(btRbInfo);

		if (isRegistPhysicsWorld) {
			//物理ワールドに登録。
			PhysicsWorld().AddRigidBody(*this);
		}
	}
}