#pragma once

class Car;
class ICarState;
class CarStateIdle;
class CarStateAccele;
class CarStateBrake;
class CarStateDrift;
class ICarDriver;

class CarMoveController
{
public:
	static const float MOVE_COEFFICIENT;				//���ԃx�[�X�̈ړ��Ɋ|����W���B

public:

	enum EnCarState {
		enStateIdle,
		enStateAccele,
		enStateBrake,
		enStateDrift,
		enStateNum
	};

public:
	CarMoveController();
	~CarMoveController();
	/// <summary>
	/// �L�����R���Ƃ��̏������̓C���X�^���X��������ɍs�������B
	/// </summary>
	void Init(const Vector3& position, const Quaternion& rotation);
	/// <summary>
	/// �Ԃ̈ړ������B
	/// </summary>
	/// <returns>�ړ��\���ǂ����̔���B</returns>
	bool MoveCar();
	/// <summary>
	/// �X�e�[�g��ύX����B
	/// </summary>
	/// <param name="state">�񋓂Ŏw����s���B</param>
	void ChangeState(EnCarState state);

public:		//�Ԃ̈ړ��Ȃǂ̏���
	/// <summary>
	/// �����x��^����B
	/// </summary>
	/// <param name="diccele">�����x�B</param>
	void AddDicceleration(const float diccele);
	/// <summary>
	/// �Ԃ̌������v�Z����B
	/// </summary>
	void CalcDirection();
	/// <summary>
	/// �����x�𑬓x�ɉ��Z����B
	/// </summary>
	/// <param name="accele">�����x�B</param>
	void AddAcceleration(const float accele)
	{
		m_speed = min(MAX_SPEED, m_speed + (accele * MOVE_COEFFICIENT * GameTime().GetFrameDeltaTime()));
	}
public:		//Set�֐��B
	/// <summary>
	/// �Ԃ̃h���C�o��ݒ肷��B
	/// </summary>
	/// <param name="driver">�h���C�o�̃A�h���X�B</param>
	void SetCarDriver(ICarDriver* driver)
	{
		m_carDriver = driver;
	}
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

public:		//Get�֐��B
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	const Vector3& GetForward()const
	{
		return m_forward;
	}
	float GetSpeed()const
	{
		return m_speed;
	}
	CCharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
	const ICarDriver* GetCarDriver()const
	{
		return m_carDriver;
	}

public:
	static constexpr float MAX_SPEED = 6800.0f;

private:	//�X�e�[�g��
	ICarState* m_currentState = nullptr;
	std::unique_ptr<ICarState> m_carStates[EnCarState::enStateNum];
	//�Ԃ̑���̃h���C�o�B
	ICarDriver* m_carDriver = nullptr;
	//�ړ��֌W�B
	CCharacterController m_charaCon;		//�L�����R���B
	Vector3 m_position = Vector3::Zero;		//���W�B
	//�Ԃ̌���
	Vector3 m_forward = Vector3::AxisZ;		//�Ԃ̑O�����B
	Vector3 m_right = Vector3::AxisX;		//�Ԃ̉E�����B
	Vector3 m_up = Vector3::AxisY;			//�Ԃ̏�����B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
	Vector3 m_moveSpeed = Vector3::Zero;	//�ړ����x�B
	float m_speed = 0.0f;					//���x�B
};

