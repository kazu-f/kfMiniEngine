#pragma once

#include "RigidBody.h"
#include "PhysicsDebugDraw.h"

namespace Engine {
	class CCharacterController;

	class CPhysicsWorld
	{
		std::unique_ptr<btDefaultCollisionConfiguration>	 m_collisionConfig;		//コリジョンの設定？
		std::unique_ptr<btCollisionDispatcher>				 m_collisionDispatcher;	//衝突解決処理。
		std::unique_ptr<btBroadphaseInterface>				 m_overlappingPairCache;	//ブロードフェーズ。衝突判定の枝切り。？
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_constraintSolver;	//コンストレイントソルバー。拘束条件の解決処理。？
		std::unique_ptr<btDiscreteDynamicsWorld>			 m_dynamicWorld;		//物理ワールド。

#ifdef _DEBUG
		CPhysicsDebugDraw m_debugDraw;												//ワイヤーフレーム表示。
#endif
	public:
		~CPhysicsWorld();
		void Init();		//初期化。
		void Update(const float time);		//物理ワールドの更新。
		void DebugDrawWorld(RenderContext& rc);		//デバッグ描画。
		void Release();		//解放処理。
		/// <summary>
		/// 重力の設定。
		/// </summary>
		void SetGravity(Vector3 g)
		{
			m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
		}
		/// <summary>
		/// ダイナミックワールドの取得。
		/// </summary>
		btDiscreteDynamicsWorld* GetDynamicWorld()
		{
			return m_dynamicWorld.get();
		}
		/// <summary>
		/// デバッグモードを設定する。
		/// </summary>
		/// <param name="debugMode"></param>
		void SetDebugMode(int debugMode)
		{
#ifdef _DEBUG
			m_debugDraw.setDebugMode(debugMode);
#endif
		}

		/// <summary>
		/// 剛体を登録。
		/// </summary>
		void AddRigidBody(CRigidBody& rb)
		{
			//登録されていないなら登録。
			if (rb.IsAddPhysicsWorld() == false) {
				m_dynamicWorld->addRigidBody(rb.GetBody());
				rb.SetMarkAddPhysicsWorld();
			}
		}
		/// <summary>
		/// 剛体を破棄。
		/// </summary>
		void RemoveRigidBody(CRigidBody& rb)
		{
			//登録されているなら破棄。
			if (rb.IsAddPhysicsWorld() == true) {
				m_dynamicWorld->removeRigidBody(rb.GetBody());
				rb.SetUnMarkAddPhysicsWorld();
			}
		}
		/// <summary>
		/// コリジョンオブジェクトをワールドに登録。
		/// </summary>
		/// <param name="colliObj">コリジョンオブジェクト。</param>
		void AddCollisionObject(btCollisionObject& colliObj)
		{
			m_dynamicWorld->addCollisionObject(&colliObj);
		}
		/// <summary>
		/// コリジョンオブジェクトをワールドから削除。
		/// </summary>
		/// <param name="colliObj">コリジョンオブジェクト。</param>
		void RemoveCollisionObject(btCollisionObject& colliObj)
		{
			m_dynamicWorld->removeCollisionObject(&colliObj);
		}
		/// <summary>
		/// 衝突検出を行う。
		/// </summary>
		/// <param name="castShape">検出を行う剛体</param>
		/// <param name="convexFromWorld">移動の始点。</param>
		/// <param name="convextoWorld">移動の終点</param>
		/// <param name="resultCallback">コールバック用関数オブジェクト</param>
		/// <param name="allowedCcdPenetration">なんだろ?</param>
		void ConvexSweepTest(
			const btConvexShape* castShape,
			const btTransform& convexFromWorld,
			const btTransform& convextoWorld,
			btCollisionWorld::ConvexResultCallback& resultCallback,
			btScalar allowedCcdPenetration = 0.0f
		) {
			m_dynamicWorld->convexSweepTest(
				castShape,
				convexFromWorld,
				convextoWorld,
				resultCallback,
				allowedCcdPenetration
			);
		}
		/// <summary>
		/// 接触しているか判定。
		/// </summary>
		/// <param name="colObj">コリジョン。</param>
		/// <param name="cb">判定後の関数。</param>
		void ContactTest(
			btCollisionObject* colObj,
			std::function<void(const btCollisionObject& contactCollisionObject)> cb
		);
		/// <summary>
		/// 接触しているか判定。
		/// </summary>
		/// <param name="rb">リジッドボディ。</param>
		/// <param name="cb">判定後の関数。</param>
		void ContactTest(
			CRigidBody& rb,
			std::function<void(const btCollisionObject& contactCollisionObject)> cb
		);
		/// <summary>
		/// 接触しているか判定。
		/// </summary>
		/// <param name="charaCon">キャラコン。</param>
		/// <param name="cb">判定後の関数。</param>
		void ContactTest(
			CCharacterController& charaCon,
			std::function<void(const btCollisionObject& contactCollisionObject)> cb	
		);

	};///class CPhysicsWorld

}///namespace Engine;