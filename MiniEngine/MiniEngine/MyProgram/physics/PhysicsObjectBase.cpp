#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "physics/BoxCollider.h"
#include "physics/CapsuleCollider.h"
#include "physics/SphereCollider.h"
#include "physics/MeshCollider.h"
#include "graphics/Model.h"
#include "prefab/ModelRender.h"

namespace Engine {
	void CPhysicsObjectBase::CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& size)
	{
		Release();
		//ボックスコライダーを作成。
		auto boxCollider = std::make_unique<CBoxCollider>();
		boxCollider->Create(size);
		m_sphere = std::move(boxCollider);
		//物理オブジェクトの作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateCapsule(const Vector3& pos, const Quaternion& rot, const float radius, const float height)
	{
		Release();
		//カプセルコライダーを作成。
		auto capsuleCollider = std::make_unique<CCapsuleCollider>();
		capsuleCollider->Create(radius, height);
		m_sphere = std::move(capsuleCollider);
		//物理オブジェクトの作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateSphere(const Vector3& pos, const Quaternion& rot, const float radius)
	{
		Release();
		//球体コライダーの作成。
		auto sphereCollider = std::make_unique<CSphereCollider>();
		sphereCollider->Create(radius);
		m_sphere = std::move(sphereCollider);
		//物理オブジェクトの作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateMesh(const Vector3& pos, const Quaternion& rot, const TkmFile& tkmFile)
	{
		Release();
		//メッシュコライダーの作成。
		auto meshCollider = std::make_unique<CMeshCollider>();
		meshCollider->CreateFromTkmFile(tkmFile, nullptr);
		m_sphere = std::move(meshCollider);
		//物理オブジェクト作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateMesh(const Vector3& pos, const Quaternion& rot, const Vector3 & scale, const Model& model)
	{
		Release();
		//スケールを調整するための行列作成。
		Matrix mScale;
		mScale.MakeScaling(scale);
		//メッシュコライダーの作成。
		auto meshCollider = std::make_unique<CMeshCollider>();
		meshCollider->CreateFromModel(model, &mScale);
		m_sphere = std::move(meshCollider);
		//物理オブジェクトの作成、登録。
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateMesh(const Vector3& pos, const Quaternion& rot, const Vector3& scale, prefab::ModelRender* modelRender)
	{
		//モデルから作成。
		CreateMesh(pos, rot, scale, modelRender->GetModel());
	}
}