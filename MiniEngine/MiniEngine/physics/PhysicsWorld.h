#pragma once

#include "RigidBody.h"

namespace Engine {

	class CPhysicsWorld
	{
		std::unique_ptr<btDefaultCollisionConfiguration>	 m_collisionConfig;		//�R���W�����̐ݒ�H
		std::unique_ptr<btCollisionDispatcher>				 m_collisionDispatcher;	//�Փˉ��������B
		std::unique_ptr<btBroadphaseInterface>				 m_overlappingPairCache;	//�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B�H
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_constraintSolver;	//�R���X�g���C���g�\���o�[�B�S�������̉��������B�H
		std::unique_ptr<btDiscreteDynamicsWorld>			 m_dynamicWorld;		//�������[���h�B

	public:
		~CPhysicsWorld();
		void Init();		//�������B
		void Update(const float time);		//�������[���h�̍X�V�B
		void Release();		//��������B
		/// <summary>
		/// �d�͂̐ݒ�B
		/// </summary>
		void SetGravity(Vector3 g)
		{
			m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
		}
		/// <summary>
		/// �_�C�i�~�b�N���[���h�̎擾�B
		/// </summary>
		btDiscreteDynamicsWorld* GetDynamicWorld()
		{
			return m_dynamicWorld.get();
		}
		/// <summary>
		/// ���̂�o�^�B
		/// </summary>
		void AddRigidBody(CRigidBody& rb)
		{
			//�o�^����Ă��Ȃ��Ȃ�o�^�B
			if (rb.IsAddPhysicsWorld() == false) {
				m_dynamicWorld->addRigidBody(rb.GetBody());
				rb.SetMarkAddPhysicsWorld();
			}
		}
		/// <summary>
		/// ���̂�j���B
		/// </summary>
		void RemoveRigidBody(CRigidBody& rb)
		{
			//�o�^����Ă���Ȃ�j���B
			if (rb.IsAddPhysicsWorld() == true) {
				m_dynamicWorld->removeRigidBody(rb.GetBody());
				rb.SetUnMarkAddPhysicsWorld();
			}
		}

	};///class CPhysicsWorld

}///namespace Engine;