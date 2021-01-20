#include "stdafx.h"
#include "CharacterController.h"
#include "physics/CollisionAttr.h"

namespace Engine {

	namespace {
		//衝突したときに呼ばれる関数オブジェクト(地面用)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;							//衝突フラグ。
			Vector3 hitPos = { 0.0f,-FLT_MAX,0.0f };	//衝突点。
			Vector3 startPos = Vector3::Zero;			//レイの視点。
			Vector3 hitNormal = Vector3::Zero;			//衝突点の法線。
			btCollisionObject* me = nullptr;			//自分自身との衝突を除外するためのメンバ。
			float dist = FLT_MAX;						//衝突点までの距離。一番近い衝突点を求めるため、最大値を初期値にしておく。

			//衝突したときに呼ばれるコールバック関数。
			virtual btScalar	addSingleResult(
				btCollisionWorld::LocalConvexResult& convexResult, 
				bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					//自分に衝突 or キャラクタ属性と衝突 or ゴーストオブジェクトと衝突。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
				//上方向と法線のなす角度を求める。
				float angle = hitNormalTmp.Dot(Vector3::Up);	//上ベクトルと内積を取る。
				angle = fabsf(acosf(angle));					//ラジアン単位の角度に直す。
				if (angle < Math::PI * 0.3f						//地面の傾斜が一定より小さいため地面とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground		//コリジョン属性が地面と指定されている。
					) {
					//衝突している。
					isHit = true;
					Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
					//衝突点の距離を求める。
					Vector3 vDist;		//レイの始点から衝突点までのベクトル。
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					//衝突点の距離の比較。
					if (dist > distTmp){
						//この衝突点の方が近いため、更新。
						hitPos = hitPosTmp;
						hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
						dist = distTmp;
					}
				}
				return 0.0f;
			}///btScalar addSingleResult
		};///struct SweepResultGround
		//衝突したときに呼ばれる関数オブジェクト(壁用)
		struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;							//衝突フラグ。
			Vector3 hitPos = { 0.0f,-FLT_MAX,0.0f };	//衝突点。
			Vector3 startPos = Vector3::Zero;			//レイの視点。
			Vector3 hitNormal = Vector3::Zero;			//衝突点の法線。
			btCollisionObject* me = nullptr;			//自分自身との衝突を除外するためのメンバ。
			float dist = FLT_MAX;						//衝突点までの距離。一番近い衝突点を求めるため、最大値を初期値にしておく。

			//衝突したときに呼ばれるコールバック関数。
			virtual btScalar	addSingleResult(
				btCollisionWorld::LocalConvexResult& convexResult, 
				bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					//自分に衝突 or ゴーストオブジェクトに衝突。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				Vector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//上方向と衝突点の法線のなす角度を求める。
				float angle = hitNormalTmp.Dot(Vector3::Up);		//上ベクトルと内積をとる。
				angle = fabsf(acosf(angle));						//ラジアン角度に変換。
				if (angle >= Math::PI * 0.3f				//地面との傾斜が一定以上なので壁とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character		//コリジョンがキャラクタ属性なため壁とみなす。
					) {
					//衝突した。
					isHit = true;
					Vector3 hitPosTmp;
					hitPosTmp.Set(convexResult.m_hitPointLocal);
					//交点との距離を調べる。
					Vector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					vDist.y = 0.0f;
					float distTmp = vDist.Length();
					//衝突点の距離の比較。
					if (distTmp < dist) {
						//この衝突点のほうが近いため更新。
						hitPos = hitPosTmp;
						dist = distTmp;
						hitNormal = hitNormalTmp;
					}
				}
				return 0.0f;
			}///btScalar addSingleResult
		};///struct SweepResultWall
	}

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

