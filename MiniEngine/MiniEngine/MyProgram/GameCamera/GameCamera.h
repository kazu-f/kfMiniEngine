#pragma once

class CGameCamera : public IGameObject
{
public:
	bool Start()override final;		//�����������B
	void Update()override final;	//�X�V�����B
public:
	/// <summary>
	/// �^�[�Q�b�g�̍��W��ݒ�B
	/// </summary>
	void SetTarget(const Vector3& target)
	{
		m_target = target;
	}
private:
	void MoveTarget();

private:
	Vector3 m_target = Vector3::Zero;		//�^�[�Q�b�g�̍��W�B
	float m_targetToPosLen = 0.0f;			//�^�[�Q�b�g�܂ł̋����B
};

