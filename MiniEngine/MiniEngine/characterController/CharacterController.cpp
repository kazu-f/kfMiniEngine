#include "stdafx.h"
#include "CharacterController.h"
#include "physics/CollisionAttr.h"

namespace Engine {
	void CCharacterController::Init(float radius, float height, const Vector3& position)
	{
		m_position = position;
		//�R���W�����쐬�B
		m_radius = radius;
		m_height = height;
		m_collider.Create(radius, height);

		//���̂��������B
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Create(rbInfo);
		//���s�ړ����擾�B
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		PhysicsWorld().AddRigidBody(m_rigidBody);
		//�������ς݁B
		m_isInited = true;
	}
	const Vector3& CCharacterController::Execute(Vector3& moveSpeed, float deltaTime)
	{
		// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
		return Vector3::Zero;
	}
	void CCharacterController::RemoveRigidBody()
	{
		//���S�������Ƃ�ʒm�B
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
}

