#pragma once

#include "PhysicsObjectBase.h"

namespace Engine
{
	/// <summary>
	/// ゴーストオブジェクト
	/// </summary>
	/// <remarks>
	/// 使用方法
	/// Collisionを作成した後、
	/// PhysicsWorld().ContactTest()で接触判定を取りたい相手を引数に渡し、
	/// 関数オブジェクトを用いて自身と接触しているかを判定する。
	/// </remarks>
	class CPhysicsGhostObject:public CPhysicsObjectBase
	{
	public:
		~CPhysicsGhostObject()
		{
			Release();
		}
		/// <summary>
		/// 解放。
		/// </summary>
		void Release()override final;
		
		/// <summary>
		/// 引数で渡されたゴーストオブジェクトが自分自身か判定。
		/// </summary>
		bool IsSelf(const btCollisionObject& ghost) const
		{
			return &ghost == &m_ghostObject;
		}
		/// <summary>
		/// 座標を設定。
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			auto& btTrans = m_ghostObject.getWorldTransform();
			btVector3 btPos;
			pos.CopyTo(btPos);
			btTrans.setOrigin(btPos);
		}
		/// <summary>
		/// 回転を設定する。
		/// </summary>
		void SetRotation(const Quaternion& rot)
		{
			auto& btTrans = m_ghostObject.getWorldTransform();
			btQuaternion btRot;
			rot.CopyTo(btRot);
			btTrans.setRotation(btRot);
		}
	private:
		/// <summary>
		/// ゴースト作成処理の共通処理。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		void CreateCommon(const Vector3& pos, const Quaternion& rot)override;
	private:
		bool			m_isRegistPhysicsWorld = false;		//物理ワールドに登録済みか？
		btGhostObject	m_ghostObject;						//ゴーストオブジェクト。
	};///class CGhostObject;
}///namespace Engine;

