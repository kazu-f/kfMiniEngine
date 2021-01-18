#include "stdafx.h"
#include "CharacterController.h"
#include "physics/CollisionAttr.h"

namespace Engine {
	void CCharacterController::Init(float radius, float height, const Vector3& position)
	{
		m_position = position;
		//コリジョン作成。
		m_radius = radius;
		m_height = height;
		m_collider.Create(radius, height);

		//剛体を初期化。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Create(rbInfo);
		//平行移動を取得。
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		PhysicsWorld().AddRigidBody(m_rigidBody);
		//初期化済み。
		m_isInited = true;
	}
	const Vector3& CCharacterController::Execute(Vector3& moveSpeed, float deltaTime)
	{
		// TODO: return ステートメントをここに挿入します
		return Vector3::Zero;
	}
	void CCharacterController::RemoveRigidBody()
	{
		//死亡したことを通知。
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
}

