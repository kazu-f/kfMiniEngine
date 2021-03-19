#pragma once

#include "ICollider.h"

namespace Engine {

	class ICollider;

	/// <summary>
	/// ���̏��B
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
		Vector3 pos;			//���W
		Quaternion rot;			//��]
		Vector3 localInteria;	//�����e���\���B
		ICollider* collider;	//�R���C�_�[�B
		float mass;				//���ʁB
	};///struct RigidBodyInfo

	/// <summary>
	/// ���̃N���X�B
	/// </summary>
	class CRigidBody
	{					//�������[���h�ɒǉ�����Ă��邩�H
	public:
		~CRigidBody();
		void Release();		//����B
		void Create(RigidBodyInfo& rbInfo);		//���̂��쐬�B
		//���̂��擾�B
		btRigidBody* GetBody()
		{
			return m_rigidBody.get();
		}
		//�������[���h�ɓo�^�����������B
		void SetMarkAddPhysicsWorld()
		{
			m_isAddPhysicsWorld = true;
		}
		//�������[���h�ɓo�^���̈���O���B
		void SetUnMarkAddPhysicsWorld()
		{
			m_isAddPhysicsWorld = false;
		}
		//�������[���h�ɒǉ�����Ă��邩�H
		bool IsAddPhysicsWorld() const
		{
			return m_isAddPhysicsWorld;
		}
		/// <summary>
		/// �����I�u�W�F�N�g�̍��W�Ɖ�]��ݒ�B
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
		std::unique_ptr<btRigidBody>		m_rigidBody;		//���́B
		std::unique_ptr<btDefaultMotionState>	m_myMotionState;//���[�V�����X�e�[�g�B�H
		bool m_isAddPhysicsWorld = false;

	};///class CRigidBody;

}///namespace Engine;
