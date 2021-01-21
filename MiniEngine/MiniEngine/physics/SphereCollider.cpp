#include "stdafx.h"
#include "SphereCollider.h"

namespace Engine {
	CSphereCollider::CSphereCollider()
	{
	}
	CSphereCollider::~CSphereCollider()
	{
	}
	void CSphereCollider::Create(const float radius)
	{
		shape = std::make_unique<btSphereShape>(radius);
	}
}