#include "stdafx.h"
#include "CharacterController.h"
#include "physics/CollisionAttr.h"

namespace Engine {

	namespace {
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
				Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
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
						hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
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

