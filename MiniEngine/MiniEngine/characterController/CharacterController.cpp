#include "stdafx.h"
#include "CharacterController.h"
#include "physics/CollisionAttr.h"

namespace Engine {

	namespace {
		//1m�̃X�P�[����ݒ肷��B
		const float SCALE_1M = 100.0f;		//100.0cm

		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;							//�Փ˃t���O�B
			Vector3 hitPos = { 0.0f,-FLT_MAX,0.0f };	//�Փ˓_�B
			Vector3 startPos = Vector3::Zero;			//���C�̎��_�B
			Vector3 hitNormal = Vector3::Zero;			//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;			//�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
			float dist = FLT_MAX;						//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁A�ő�l�������l�ɂ��Ă����B

			//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual btScalar	addSingleResult(
				btCollisionWorld::LocalConvexResult& convexResult, 
				bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					//�����ɏՓ� or �L�����N�^�����ƏՓ� or �S�[�X�g�I�u�W�F�N�g�ƏՓˁB
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				Vector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//������Ɩ@���̂Ȃ��p�x�����߂�B
				float angle = hitNormalTmp.Dot(Vector3::Up);	//��x�N�g���Ɠ��ς����B
				angle = fabsf(acosf(angle));					//���W�A���P�ʂ̊p�x�ɒ����B
				if (angle < Math::PI * 0.3f						//�n�ʂ̌X�΂�����菬�������ߒn�ʂƂ݂Ȃ��B
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground		//�R���W�����������n�ʂƎw�肳��Ă���B
					) {
					//�Փ˂��Ă���B
					isHit = true;
					Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
					//�Փ˓_�̋��������߂�B
					Vector3 vDist;		//���C�̎n�_����Փ˓_�܂ł̃x�N�g���B
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					//�Փ˓_�̋����̔�r�B
					if (dist > distTmp){
						//���̏Փ˓_�̕����߂����߁A�X�V�B
						hitPos = hitPosTmp;
						hitNormal = hitNormalTmp;
						dist = distTmp;
					}
				}
				return 0.0f;
			}///btScalar addSingleResult
		};///struct SweepResultGround
		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
		struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;							//�Փ˃t���O�B
			Vector3 hitPos = { 0.0f,-FLT_MAX,0.0f };	//�Փ˓_�B
			Vector3 startPos = Vector3::Zero;			//���C�̎��_�B
			Vector3 hitNormal = Vector3::Zero;			//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;			//�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
			float dist = FLT_MAX;						//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁A�ő�l�������l�ɂ��Ă����B

			//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual btScalar	addSingleResult(
				btCollisionWorld::LocalConvexResult& convexResult, 
				bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					//�����ɏՓ� or �S�[�X�g�I�u�W�F�N�g�ɏՓˁB
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				Vector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
				float angle = hitNormalTmp.Dot(Vector3::Up);		//��x�N�g���Ɠ��ς��Ƃ�B
				angle = fabsf(acosf(angle));						//���W�A���p�x�ɕϊ��B
				if (angle >= Math::PI * 0.3f				//�n�ʂƂ̌X�΂����ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character		//�R���W�������L�����N�^�����Ȃ��ߕǂƂ݂Ȃ��B
					) {
					//�Փ˂����B
					isHit = true;
					Vector3 hitPosTmp;
					hitPosTmp.Set(convexResult.m_hitPointLocal);
					//��_�Ƃ̋����𒲂ׂ�B
					Vector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					vDist.y = 0.0f;
					float distTmp = vDist.Length();
					//�Փ˓_�̋����̔�r�B
					if (distTmp < dist) {
						//���̏Փ˓_�̂ق����߂����ߍX�V�B
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
		if (m_isInited == false)
		{
			_WARNING_MESSAGE("�L�����R���̏�����������Ă��Ȃ��B");
			return Vector3::Zero;
		}
		if (moveSpeed.y > 0.0f) {
			//������ђ��B
			m_isJump = true;
			m_isOnGround = false;
		}
		//���̈ړ���̍��W���v�Z����B
		Vector3 nextPosition = m_position;
		//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B
		Vector3 moveVec = moveSpeed;
		moveVec.Scale(deltaTime);
		nextPosition.Add(moveVec);
		Vector3 originalXYDir = moveVec;
		originalXYDir.y = 0.0f;
		originalXYDir.Normalize();
		//XZ���ʏ�ł̏Փˌ��o�Ɖ������s���B
		{

		}
		//XZ�̈ړ��͊m�肷��B
		m_position.x = nextPosition.x;
		m_position.z = nextPosition.z;

		//�������̏Փˌ��o�Ɖ����B
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);

			m_position = nextPosition;		//�ړ������m��B

			//���C���쐬
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
			start.setOrigin(
				btVector3(
					m_position.x,
					m_position.y + m_height * 0.5f + m_radius, 
					m_position.z)
			);
			//�I�_�͒n�ʏ�ɂ��Ȃ��Ȃ�1m��������B
			//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
			//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
			Vector3 endPos;
			endPos.Set(start.getOrigin());
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//�W�����v���Ƃ��ŏ㏸���B
					//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\���𒲂ׂ�B
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
					endPos.y += addPos.y;
				}
			}
			else {
				//�n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
				endPos.y -= SCALE_1M;
			}
			//���C�̏I�_�̍��W��ݒ�B
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

			//���C�̔�����s���B
			SweepResultGround callback;
			callback.me = m_rigidBody.GetBody();
			callback.startPos.Set(start.getOrigin());
			//�Փˌ��o�B
			//Y���W�ɕω�������B
			if (fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON) {
				//�Փ˔���B
				PhysicsWorld().ConvexSweepTest(
					(const btConvexShape*)m_collider.GetBody(),
					start, end, callback
				);
				if (callback.isHit) {
					//���������B
					moveSpeed.y = 0.0f;
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y;
				}
				else {
					//�n�ʏ�ɂ��Ȃ��B
					m_isOnGround = false;
				}
			}
		}

		//�ړ��m��B
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//���̂��������B
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(
			m_position.x,
			m_position.y + m_height * 0.5f + m_radius,		//�R���C�_�[�̒��S�̈ʒu�܂ō�������B	
			m_position.z
		));

		//�ړ���̍��W��Ԃ��B
		return m_position;
	}
	void CCharacterController::RemoveRigidBody()
	{
		//���S�������Ƃ�ʒm�B
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
}
