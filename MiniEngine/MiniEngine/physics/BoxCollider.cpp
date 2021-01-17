#include "stdafx.h"
#include "BoxCollider.h"

namespace Engine {
	CBoxCollider::CBoxCollider()
	{
	}
	CBoxCollider::~CBoxCollider()
	{
		//解放。
		delete shape;
	}
	void CBoxCollider::Create(const Vector3& size)
	{
		//ボックスコライダーの作成。
		shape = new btBoxShape(btVector3(size.x * 0.5, size.y * 0.5f, size.z * 0.5f));
	}
}
