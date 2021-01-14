#pragma once

#include "ICollider.h"

namespace Engine {

	class ICollider;

	/// <summary>
	/// 剛体情報。
	/// </summary>
	struct RigidBodyInfo {
		RigidBodyInfo() :
			collider(NULL),
			mass(0.0f)
		{
			pos = Vector3::Zero;
			rot = Quaternion::Identity;
			localInteria = Vector3::Zero;
		}
		Vector3 pos;			//座標
		Quaternion rot;			//回転
		Vector3 localInteria;	//慣性テンソル。
		ICollider* collider;	//コライダー。
		float mass;				//質量。
	};///struct RigidBodyInfo

	/// <summary>
	/// 剛体クラス。
	/// </summary>
	class CRigidBody
	{					//物理ワールドに追加されているか？
	public:
		~CRigidBody();
		void Release();		//解放。
		void Create(RigidBodyInfo& rbInfo);		//剛体を作成。
		//剛体を取得。
		btRigidBody* GetBody()
		{
			return m_rigidBody.get();
		}
		//物理ワールドに登録した印をつける。
		void SetMarkAddPhysicsWorld()
		{
			m_isAddPhysicsWorld = true;
		}
		//物理ワールドに登録中の印を外す。
		void SetUnMarkAddPhysicsWorld()
		{
			m_isAddPhysicsWorld = false;
		}
		//物理ワールドに追加されているか？
		bool IsAddPhysicsWorld() const
		{
			return m_isAddPhysicsWorld;
		}
		/// <summary>
		/// 物理オブジェクトの座標と回転を設定。
		/// </summary>
		void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
		{
			btTransform trans;
			btVector3 btPos;
			pos.CopyTo(btPos);
			trans.setOrigin(btPos);
			btQuaternion btRot;
			rot.CopyTo(btRot);
			trans.setRotation(btRot);
			m_rigidBody->setWorldTransform(trans);
		}

	private:
		std::unique_ptr<btRigidBody>		m_rigidBody;		//剛体。
		std::unique_ptr<btDefaultMotionState>	m_myMotionState;//モーションステート。？
		bool m_isAddPhysicsWorld = false;

	};///class CRigidBody;

}///namespace Engine;
