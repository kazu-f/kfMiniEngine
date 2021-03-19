#include "stdafx.h"
#include "PhysicsStaticObject.h"


namespace Engine {
	void CPhysicsStaticObject::Release()
	{
		//„‘Ì‚Ì“o˜^‚ğ‰ğœB
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
	void CPhysicsStaticObject::CreateCommon(const Vector3& pos, const Quaternion& rot)
	{
		//„‘Ì‚ğì¬B
		RigidBodyInfo rbInfo;
		rbInfo.collider = m_collider.get();
		rbInfo.mass = 0.0f;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		m_rigidBody.Create(rbInfo);
		//„‘Ì‚ğ“o˜^B
		PhysicsWorld().AddRigidBody(m_rigidBody);
	}
}

