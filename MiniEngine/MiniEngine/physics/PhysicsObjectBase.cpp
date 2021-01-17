#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "physics/BoxCollider.h"
#include "graphics/Model.h"
#include "prefab/ModelRender.h"

namespace Engine {
	void CPhysicsObjectBase::CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& size)
	{
		Release();
		//�{�b�N�X�R���C�_�[���쐬�B
		auto boxCollider = std::make_unique<CBoxCollider>();
		boxCollider->Create(size);
		m_collider = std::move(boxCollider);
		//�����I�u�W�F�N�g�̍쐬�A�o�^�B
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