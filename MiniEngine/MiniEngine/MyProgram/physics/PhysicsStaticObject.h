#pragma once

#include "physics/RigidBody.h"
#include "physics/PhysicsObjectBase.h"

namespace Engine {

	class CPhysicsStaticObject : public CPhysicsObjectBase
	{
	public:
		~CPhysicsStaticObject()
		{
			Release();
		}
		/// <summary>
		/// 解放処理。
		/// </summary>
		void Release()override final;

		/// <summary>
		/// 座標と回転を設定。
		/// </summary>
		void SetPosAndRot(const Vector3& pos, const Quaternion& rot)
		{
			m_rigidBody.SetPositionAndRotation(pos, rot);
		}
		/// <summary>
		/// 剛体を取得。
		/// </summary>
		CRigidBody& GetRigidBody()
		{
			return m_rigidBody;
		}

	private:
		/// <summary>
		/// 静的物理オブジェクト作成の共通処理。
		/// </summary>
		void CreateCommon(const Vector3& pos, const Quaternion& rot)override final;
	private:
		CRigidBody m_rigidBody;			//剛体。
	};///class CPhysicsStaticObject

}///namespace Engine

