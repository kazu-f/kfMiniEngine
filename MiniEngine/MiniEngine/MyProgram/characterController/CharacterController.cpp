#include "stdafx.h"
#include "CharacterController.h"
#include "physics/CollisionAttr.h"

namespace Engine {

	namespace {
		//1mのスケールを設定する。
		const float SCALE_1M = 100.0f;		//100.0cm

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
				Vector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
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
						hitNormal = hitNormalTmp;
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
						hitNormal = hitNormalTmp;
						dist = distTmp;
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
		m_sphere.Create(radius, height);

		//剛体を初期化。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_sphere;
		rbInfo.mass = 0.0f;
		m_rigidBody.Create(rbInfo);
		//平行移動を取得。
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		//初期化済み。
		m_isInited = true;
	}
	const Vector3& CCharacterController::Execute(Vector3& moveSpeed, float deltaTime)
	{
		if (m_isInited == false)
		{
			_WARNING_MESSAGE("キャラコンの初期化がされていない。");
			return Vector3::Zero;
		}
		if (moveSpeed.y > 0.0f) {
			//吹っ飛び中。
			m_isJump = true;
			m_isOnGround = false;
		}
		//次の移動先の座標を計算する。
		Vector3 nextPosition = m_position;
		//速度からこのフレームでの移動量を求める。
		Vector3 moveVec = moveSpeed;
		moveVec.Scale(deltaTime);
		nextPosition.Add(moveVec);
		Vector3 originalXYDir = moveVec;
		originalXYDir.y = 0.0f;
		originalXYDir.Normalize();
		//XZ平面上での衝突検出と解決を行う。
		{
			int loopCount = 0;
			while (true) {
				//現在の座標から次の移動先へ向かうベクトルを求める。
				Vector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				Vector3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (addPosXZ.Length() < FLT_EPSILON) {
					//XZ平面で動きがないため検出不要。
					break;
				}

				//カプセルコライダーの中心座標 + 高さ*0.1の座標。
				Vector3 posTmp = m_position;
				posTmp.y += m_height * 0.6f + m_radius;
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心より少し上。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//終点は次の移動先とする。XZ平面での検出のためyは始点と同じ。
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				SweepResultWall callback;
				callback.me = m_rigidBody.GetBody();
				callback.startPos = posTmp;
				//衝突検出。
				PhysicsWorld().ConvexSweepTest(
					static_cast<const btConvexShape*>(m_sphere.GetBody()),
					start, end, callback
				);

				if (callback.isHit) {
					//当たった。
					Vector3 vT0, vT1;
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);			//移動後の座標。
					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);	//衝突点。
					//めり込みが発生している移動ベクトル。
					Vector3 vMerikomi;
					vMerikomi.Subtract(vT0, vT1);
					//XZ平面での衝突した壁の法線を求める。
					Vector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//めり込みベクトルを壁の法線に射影する。
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//押し戻すベクトルを求める。
					//押し戻すベクトル = hitNormalXZ * (fT0 + radius)
					Vector3 vOffset;
					vOffset = hitNormalXZ;
					vOffset.Scale(-fT0 + m_radius);
					nextPosition.Add(vOffset);
					//移動方向。
					Vector3 currentDir;
					currentDir.Subtract(nextPosition, m_position);
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXYDir) < 0.0f) {
						//角に入ったときのキャラの振動を防止。
						//移動先が逆向きになったら移動をキャンセル。
						nextPosition.x = m_position.x;
						nextPosition.z = m_position.z;
						break;
					}
				}
				else {
					//どことも当たらないので終わる。
					break;
				}
				//一定数以上は行わない。
				loopCount++;
				if (loopCount == 5) {
					break;
				}

			}
		}
		//XZの移動は確定する。
		m_position.x = nextPosition.x;
		m_position.z = nextPosition.z;

		//下方向の衝突検出と解決。
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);

			m_position = nextPosition;		//移動を仮確定。

			//レイを作成
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心。
			start.setOrigin(
				btVector3(
					m_position.x,
					m_position.y + m_height * 0.5f + m_radius, 
					m_position.z)
			);
			//終点は地面上にいないなら1m下を見る。
			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
			Vector3 endPos;
			endPos.Set(start.getOrigin());
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//ジャンプ中とかで上昇中。
					//上昇中でもXZに移動した結果めり込んでいる可能性を調べる。
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					//落下している場合はそのまま下を調べる。
					endPos.y += addPos.y;
				}
			}
			else {
				//地面上にいる場合は1m下を見る。
				endPos.y -= SCALE_1M;
			}
			//レイの終点の座標を設定。
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

			//レイの判定を行う。
			SweepResultGround callback;
			callback.me = m_rigidBody.GetBody();
			callback.startPos.Set(start.getOrigin());
			//衝突検出。
			//Y座標に変化がある。
			if (fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON) {
				//衝突判定。
				PhysicsWorld().ConvexSweepTest(
					(const btConvexShape*)m_sphere.GetBody(),
					start, end, callback
				);
				if (callback.isHit) {
					//当たった。
					moveSpeed.y = 0.0f;
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y;
				}
				else {
					//地面上にいない。
					m_isOnGround = false;
				}
			}
		}

		//移動確定。
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体も動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(
			m_position.x,
			m_position.y + m_height * 0.5f + m_radius,		//コライダーの中心の位置まで高くする。	
			m_position.z
		));

		//移動先の座標を返す。
		return m_position;
	}
}

