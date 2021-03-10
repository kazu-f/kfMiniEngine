#pragma once

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
	void Init(CCharacterController* chara, Vector3& pos, Quaternion& rot);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	void Update(Vector3& pos, Quaternion& rot);
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
		m_currentCheckPointNum = checkPointNum;
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
	/// �L�����R�����擾����B
	/// </summary>
	CCharacterController* GetCharaCon()
	{
		return m_characon;
	}
	bool IsReverseRun()const
	{
		return m_isReverseRun;
	}

private:
	/// <summary>
	/// ��]����O�������v�Z�B
	/// </summary>
	void CalcForward();

private:
	CCharacterController* m_characon = nullptr;			//�L�����R���B
	Vector3 m_position = Vector3::Zero;					//���W�B
	Quaternion m_rotation = Quaternion::Identity;		//��]�B
	Vector3 m_forward = Vector3::AxisZ;					//�O�����B
	int m_currentCheckPointNum = 0;						//���݂̎��̃`�F�b�N�|�C���g�ԍ��B
	int m_currentLapsNum = 0;							//���݂̎��񐔁B
	bool m_isReverseRun = false;						//�t�������H
};

