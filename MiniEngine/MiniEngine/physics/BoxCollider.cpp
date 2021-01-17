#include "stdafx.h"
#include "BoxCollider.h"

namespace Engine {
	CBoxCollider::CBoxCollider()
	{
	}
	CBoxCollider::~CBoxCollider()
	{
		//����B
		delete shape;
	}
	void CBoxCollider::Create(const Vector3& size)
	{
		//�{�b�N�X�R���C�_�[�̍쐬�B
		shape = new btBoxShape(btVector3(size.x * 0.5, size.y * 0.5f, size.z * 0.5f));
	}
}
