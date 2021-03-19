#include "stdafx.h"
#include "PhysicsStaticObject.h"


namespace Engine {
	void CPhysicsStaticObject::Release()
	{
		//���̂̓o�^�������B
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
	void CPhysicsStaticObject::CreateCommon(const Vector3& pos, const Quaternion& rot)
	{
		//���̂��쐬�B
		RigidBodyInfo rbInfo;
		rbInfo.collider = m_collider.get();
		rbInfo.mass = 0.0f;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		m_rigidBody.Create(rbInfo);
		//���̂�o�^�B
		PhysicsWorld().AddRigidBody(m_rigidBody);
	}
}

