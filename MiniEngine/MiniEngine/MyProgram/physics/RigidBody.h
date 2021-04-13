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
		Vector3 localInteria;	//慣性テンソル。回転のしやすさ(0.0f ～ 1.0f)。
		ICollider* collider;	//コライダー。
		float mass;				//質量。
	};///struct RigidBodyInfo

	/// <summary>
	/// 剛体クラス。
	/// </summary>
	class CRigidBody : Noncopyable
	{					//物理ワールドに追加されているか？
	public:
		~CRigidBody();
		void Release();		//解放。
		/// <summary>
		/// 剛体を作成する。
		/// </summary>
		/// <param name="rbInfo">剛体情報。</param>
		/// <param name="isRegistPhysicsWorld">物理ワールドに登録するか？</param>
		void Create(RigidBodyInfo& rbInfo,bool isRegistPhysicsWorld = true);
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
	public:		//Get関数。
		/// <summary>
		/// 物理オブジェクトの座標と回転を取得。
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="rot"></param>
		void GetPositionAndRotation(Vector3& pos, Quaternion& rot)
		{
			btTransform trans;
			m_myMotionState->getWorldTransform(trans);
			pos.Set(trans.getOrigin());
			rot.Set(trans.getRotation());
		}


	public:		//Set関数。
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
		/// <summary>
		/// 速度を設定。
		/// </summary>
		/// <param name="vel">速度ベクトル。</param>
		void SetLinerVelocity(const Vector3& vel)
		{
			btVector3 btVel;
			btVel.setValue(vel.x, vel.y, vel.z);
			m_rigidBody->setLinearVelocity(btVel);
			m_rigidBody->activate();
		}
		/// <summary>
		/// 摩擦を設定。
		/// </summary>
		/// <param name="frict">摩擦係数？</param>
		void SetFriction(const float frict)
		{
			m_rigidBody->setFriction(frict);			//移動用の摩擦？
			m_rigidBody->setRollingFriction(frict);		//回転にも摩擦を使うらしい？
		}
		/// <summary>
		/// 移動可能軸の設定。
		/// </summary>
		/// <param name="x">X軸。</param>
		/// <param name="y">Y軸。</param>
		/// <param name="z">Z軸。</param>
		void SetLinearFactor(const float x, const float y, const float z)
		{
			Vector3 v = { x,y,z };
			SetLinearFactor(v);
		}
		/// <summary>
		/// 移動可能軸の設定。
		/// </summary>
		/// <param name="linearFactor">軸の値。</param>
		void SetLinearFactor(const Vector3& linearFactor)
		{
			btVector3 v;
			v.setValue(linearFactor.x, linearFactor.y, linearFactor.z);
			m_rigidBody->setLinearFactor(v);
		}
		/// <summary>
		/// 角速度の設定。
		/// </summary>
		/// <param name="angVel">角速度。</param>
		void SetAngularVelocity(const Vector3& angVel)
		{
			btVector3 v;
			v.setValue(angVel.x, angVel.y, angVel.z);
			m_rigidBody->setAngularVelocity(v);
			m_rigidBody->activate();
		}
		/// <summary>
		/// 回転可能軸を設定。
		/// </summary>
		/// <param name="x">X軸。</param>
		/// <param name="y">Y軸。</param>
		/// <param name="z">Z軸。</param>
		void SetAngularFactor(const float x, const float y, const float z)
		{
			SetAngularFactor({ x,y,z });
		}
		/// <summary>
		/// 回転可能軸を設定。
		/// </summary>
		/// <param name="angFactor">回転可能軸。</param>
		void SetAngularFactor(const Vector3& angFactor)
		{
			btVector3 v;
			v.setValue(angFactor.x, angFactor.y, angFactor.z);
			m_rigidBody->setAngularFactor(v);
		}

		void SetGravity(const float g)
		{
			btVector3 v;
			v.setValue(0.0f, -g, 0.0f);
			m_rigidBody->setGravity(v);
		}
		/// <summary>
		/// 力を加える。
		/// </summary>
		/// <param name="force">力。</param>
		/// <param name="relPos">加える座標。</param>
		void AddForce(const Vector3& force, const Vector3& relPos)
		{
			btVector3 btForce;
			btVector3 btRelPos;
			force.CopyTo(btForce);
			relPos.CopyTo(btRelPos);
			m_rigidBody->applyForce(btForce, btRelPos);
			m_rigidBody->activate();
		}

	private:
		std::unique_ptr<btRigidBody>		m_rigidBody;		//剛体。
		std::unique_ptr<btDefaultMotionState>	m_myMotionState;//モーションステート。？
		bool m_isAddPhysicsWorld = false;

	};///class CRigidBody;

}///namespace Engine;
