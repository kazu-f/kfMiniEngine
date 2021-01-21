#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "physics/BoxCollider.h"
#include "physics/CapsuleCollider.h"
#include "physics/SphereCollider.h"
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
		Release();
		//カプセルコライダーを作成。
		auto capsuleCollider = std::make_unique<CCapsuleCollider>();
		capsuleCollider->Create(radius, height);
		m_collider = std::move(capsuleCollider);
		//物理オブジェクトの作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateSphere(const Vector3& pos, const Quaternion& rot, const float radius)
	{
		Release();
		auto sphereCollider = std::make_unique<CSphereCollider>();
		sphereCollider->Create(radius);
		m_collider = std::move(sphereCollider);
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateMesh(const Vector3& pos, const Quaternion& rot, const TkmFile& tkmFile)
	{
	}
}