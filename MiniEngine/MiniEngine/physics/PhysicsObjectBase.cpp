#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "physics/BoxCollider.h"
#include "graphics/Model.h"
#include "prefab/ModelRender.h"

namespace Engine {
	void CPhysicsObjectBase::CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& size)
	{
		Release();
		//ボックスコライダーを作成。
		auto boxCollider = std::make_unique<CBoxCollider>();
		boxCollider->Create(size);
		m_collider = std::move(boxCollider);
		//物理オブジェクトの作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateCapsule(const Vector3& pos, const Quaternion& rot, const float radius, const float height)
	{
	}
	void CPhysicsObjectBase::CreateSphere(const Vector3& pos, const Quaternion& rot, const float radius)
	{
	}
	void CPhysicsObjectBase::CreateMesh(const Vector3& pos, const Quaternion& rot, const TkmFile& tkmFile)
	{
	}
}