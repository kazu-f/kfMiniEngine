#pragma once

#include "physics/CapsuleCollider.h"
#include "physics/RigidBody.h"


namespace Engine {
	/// <summary>
	/// �L�����N�^�[�R���g���[���[�B
	/// </summary>
	class CCharacterController
	{
	public:
		CCharacterController()
		{
		
		}
		~CCharacterController()
		{
			RemoveRigidBody();
		}
		/// <summary>
		/// �L�����N�^�[�R���g���[���[�̏������B
		/// </summary>
		/// <remarks>
		/// �J�v�Z���R���C�_�[���쐬���āA�����蔻������B
		/// </remarks>
		/// <param name="radius">�J�v�Z���R���C�_�[�̔��a�B</param>
		/// <param name="height">�J�v�Z���R���C�_�[�̍����B</param>
		/// <param name="position">�������W�B</param>
		void Init(float radius, float height, const Vector3& position);
		/// <summary>
		/// �L�����R���̈ړ������s�B
		/// </summary>
		/// <param name="moveSpeed">�ړ����x�B</param>
		/// <param name="deltaTime">�o�ߎ���(�b)�B�f�t�H���g�ł�1�t���[���̌o�ߎ���</param>
		/// <returns>�ړ���̃L�����N�^�[�̍��W�B</returns>
		const Vector3& Execute(Vector3& moveSpeed, float deltaTime = GameTime().GetFrameDeltaTime());
	public:
		/// <summary>
		/// ���W�̎擾�B
		/// </summary>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ���̂��擾�B
		/// </summary>
		CRigidBody* GetBody()
		{
			return &m_rigidBody;
		}
		/// <summary>
		/// ���̂𕨗��G���W������폜�B
		/// </summary>
		void RemoveRigidBody();

	private:
		bool				m_isInited = false;				//�������ς݁H
		Vector3				m_position = Vector3::Zero;		//���W�B
		bool				m_isJump = false;				//�W�����v�����H
		bool				m_isOnGround = true;			//�n�ʏ�ɂ��邩�H
		CCapsuleCollider	m_collider;						//�R���C�_�[�B
		float				m_radius = 0.0f;				//�J�v�Z���R���C�_�[�̔��a�B
		float				m_height = 0.0f;				//�J�v�Z���R���C�_�[�̍����B
		CRigidBody			m_rigidBody;					//���́B

	};///class CCharacterController; 

}///namespace Engine;
