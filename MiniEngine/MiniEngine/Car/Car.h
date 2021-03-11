#pragma once

class ICarState;
class CarStateIdle;
class CarStateAccele;
class CarStateBrake;
class CheckedController;

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
	friend class CarStateIdle;
	friend class CarStateBrake;
public:
	void ChangeState(ICarState* state);
public:		//Set�֐��Ƃ�
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

	float GetSpeed()const 
	{
		return m_speed;
	}
	//�`�F�b�N�R���g���[���[���擾�B
	CheckedController* GetCheckedController()
	{
		return m_checkedCon.get();
	}

public:		//�Ԃ̈ړ��Ȃǂ̏���
	/// <summary>
	/// �����x�𑬓x�ɉ��Z����B
	/// </summary>
	/// <param name="accele">�����x�B</param>
	void AddAcceleration(const float accele)
	{
		m_speed = min(MAX_SPEED, m_speed + (accele * 60.0f * GameTime().GetFrameDeltaTime()));
	}
	/// <summary>
	/// �����x��^����B
	/// </summary>
	/// <param name="diccele">�����x�B</param>
	void AddDicceleration(const float diccele)
	{
		m_speed = max(0.0f, m_speed - (diccele * 60.0f * GameTime().GetFrameDeltaTime()));
	}
	/// <summary>
	/// �Ԃ̌������v�Z����B
	/// </summary>
	void CalcDirection();

protected:
	static constexpr float MAX_SPEED = 6800.0f;
protected:
	ICarState* m_currentState = nullptr;
	std::unique_ptr<CarStateIdle> m_stateIdle;
	std::unique_ptr<CarStateAccele> m_stateAccele;
	std::unique_ptr<CarStateBrake> m_stateBrake;
protected:
	prefab::ModelRender* m_model = nullptr;
	//�ړ��֌W�B
	CCharacterController m_charaCon;		//�L�����R���B
	std::unique_ptr<CheckedController> m_checkedCon;			//�`�F�b�N�|�C���g�ɊĎ������B
	Vector3 m_position = Vector3::Zero;		//���W�B
	Vector3 m_moveSpeed = Vector3::Zero;	//�ړ����x�B
	float m_speed = 0.0f;					//���x�B
	//�Ԃ̌���
	Vector3 m_forward = Vector3::AxisZ;		//�Ԃ̑O�����B
	Vector3 m_right = Vector3::AxisX;		//�Ԃ̉E�����B
	Vector3 m_up = Vector3::AxisY;			//�Ԃ̏�����B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
};

