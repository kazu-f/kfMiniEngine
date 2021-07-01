#pragma once
#include "Race/RaceEnum.h"

/// <summary>
/// �`�F�b�N�|�C���g���画�肳�����́B
/// </summary>
class CheckedController
{
public:
	CheckedController();
	~CheckedController();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="chara">�L�����R���B</param>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	void Init(CRigidBody* rb, const Vector3& pos, const Quaternion& rot);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	void Update(const Vector3& pos, const Quaternion& rot);
	/// <summary>
	/// ���񐔂��J�E���g�A�b�v����B
	/// </summary>
	void CountUpLap()
	{
		m_currentLapsNum++;
	}
	/// <summary>
	/// ���񐔂�������B
	/// </summary>
	void CountDownLap()
	{
		m_currentLapsNum--;
	}
	/// <summary>
	/// �`�F�b�N�|�C���g�ԍ���ݒ肷��B
	/// </summary>
	/// <param name="checkPointNum"></param>
	void SetCheckPointNum(int checkPointNum)
	{
		m_currentCheckPointNo = checkPointNum;
	}
	/// <summary>
	/// �t�������ǂ����̃t���O��ݒ�B
	/// </summary>
	/// <param name="flag">true���t�����B</param>
	void SetIsReverseRun(bool flag)
	{
		m_isReverseRun = flag;
	}
public:		//�Q�b�g�֌W�B
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �O�������擾����B
	/// </summary>
	const Vector3& GetForward()const
	{
		return m_forward;
	}
	/// <summary>
	/// �E�������擾����B
	/// </summary>
	const Vector3& GetRight()const
	{
		return m_right;
	}
	/// <summary>
	/// ���݂̃`�F�b�N�|�C���g�ԍ����擾����B
	/// </summary>
	int GetCheckPointNo()
	{
		return m_currentCheckPointNo;
	}
	/// <summary>
	/// ���݂̎��񐔂��擾����B
	/// </summary>
	int GetCurrentLapNum()
	{
		return m_currentLapsNum;
	}
	/// <summary>
	/// �L�����R�����擾����B
	/// </summary>
	CRigidBody* GetBody()
	{
		return m_rigidBody;
	}
	/// <summary>
	/// �t�������H
	/// </summary>
	/// <returns>�t�����Ȃ�true�B</returns>
	bool IsReverseRun()const
	{
		return m_isReverseRun;
	}

	bool IsGoal()const
	{
		return m_currentLapsNum >= Race::enLap_Num;
	}

private:
	/// <summary>
	/// ��]����������v�Z�B
	/// </summary>
	void CalcDirection();

private:
	CRigidBody* m_rigidBody = nullptr;					//������s�����́B
	Vector3 m_position = Vector3::Zero;					//���W�B
	Quaternion m_rotation = Quaternion::Identity;		//��]�B
	Vector3 m_forward = Vector3::AxisZ;					//�O�����B
	Vector3 m_right = Vector3::AxisX;					//�E�����B
	int m_currentCheckPointNo = 0;						//���݂̎��̃`�F�b�N�|�C���g�ԍ��B
	int m_currentLapsNum = 0;							//���݂̎��񐔁B
	bool m_isReverseRun = false;						//�t�������H
};

