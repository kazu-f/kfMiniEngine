#include "stdafx.h"
#include "PhysicsStaticObject.h"


namespace Engine {
	void CPhysicsStaticObject::Release()
	{
	}
	void CPhysicsStaticObject::CreateCommon(const Vector3& pos, const Quaternion& rot)
	{
		//çÑëÃÇçÏê¨ÅB
		RigidBodyInfo rbInfo;
		rbInfo.collider = m_sphere.get();
		rbInfo.mass = 0.0f;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		m_rigidBody.Create(rbInfo);
	}
}

