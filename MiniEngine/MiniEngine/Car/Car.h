#pragma once

class CGameCamera;
class ICarState;
class CarStateIdle;
class CarStateAccele;

class Car : public IGameObject
{
public:
	Car();
	~Car();
	bool Start() override;			//�����������B
	void Update() override;			//�X�V�����B
	void OnDestroy() override;		//�폜�������B

	friend class ICarState;
	friend class CarStateAccele;
public:		//Set�֐��Ƃ�
	void SetCamera(CGameCamera* camera)
	{
		m_camera = camera;
	}
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

public:		//Get�֐��Ƃ�
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	const Vector3& GetForward()const
	{
		return m_forward;
	}

public:		//�Ԃ̈ړ��Ȃǂ̏���
	/// <summary>
	/// �����x�𑬓x�ɉ��Z����B
	/// </summary>
	/// <param name="accele">�����x�B</param>
	void AddAcceleration(const Vector3& accele)
	{
		m_moveSpeed += accele;
	}
	/// <summary>
	/// �Ԃ̌������v�Z����B
	/// </summary>
	void CalcDirection();

private:
	CGameCamera* m_camera = nullptr;
protected:
	ICarState* m_currentState = nullptr;
	CarStateIdle m_stateIdle;
	CarStateAccele m_stateAccele;
protected:
	prefab::ModelRender* m_model = nullptr;
	CCharacterController m_charaCon;		//�L�����R���B
	Vector3 m_position = Vector3::Zero;		//���W�B
	Vector3 m_moveSpeed = Vector3::Zero;	//�ړ����x�B
	Vector3 m_forward = Vector3::AxisZ;		//�Ԃ̑O�����B
	Vector3 m_right = Vector3::AxisX;		//�Ԃ̉E�����B
	Vector3 m_up = Vector3::AxisY;			//�Ԃ̏�����B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
};

