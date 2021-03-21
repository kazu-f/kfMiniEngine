#pragma once

#include "CheckedController.h"

/// <summary>
/// �`�F�b�N�|�C���g�B
/// </summary>
class CheckPointBase
{
public:
	CheckPointBase();
	virtual ~CheckPointBase();
	//�����������B
	void Init();
	//�L�����R���Ƃ̔���B
	virtual void CheckHit(CheckedController* checkedCon) = 0;
	/// <summary>
	/// �t������B
	/// </summary>
	/// <param name="car">�`�F�b�N�������́B</param>
	/// <returns>���茋�ʁBtrue���t�����Ă���B</returns>
	bool CheckReverseRun(CheckedController* checkedCon);

public:		//�Z�b�g�֌W�B
	//���W�̐ݒ�B
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	//��]�̐ݒ�B
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	//�`�F�b�N�|�C���g�ԍ���ݒ�B
	void SetCheckPointNum(int num)
	{
		m_checkPointNo = num;
	}
	/// <summary>
	/// �O�̃`�F�b�N�|�C���g���w��B
	/// </summary>
	void SetBackCheckPoint(CheckPointBase* back)
	{
		m_backPoint = back;
	}
	/// <summary>
	/// ���̃`�F�b�N�|�C���g���w��B
	/// </summary>
	void SetNextCheckPoint(CheckPointBase* next)
	{
		m_nextPoint = next;
	}
public:		//�Q�b�g�֌W�B
	//���W���擾�B
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	//�`�F�b�N�|�C���g�ԍ����擾�B
	int GetCheckPointNum()
	{
		return m_checkPointNo;
	}

protected:
	CPhysicsGhostObject m_ghostObj;					//�S�[�X�g�I�u�W�F�N�g�B
	Vector3 m_position = Vector3::Zero;				//���W�B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
	Vector3 m_forward = Vector3::AxisZ;				//�`�F�b�N�|�C���g�̑O�����B
	CheckPointBase* m_backPoint = nullptr;			//���̃`�F�b�N�|�C���g�B
	CheckPointBase* m_nextPoint = nullptr;			//���̃`�F�b�N�|�C���g�B
	int m_checkPointNo = -1;						//�`�F�b�N�|�C���g�ԍ��B
};


