#pragma once
#include "physics/BoxCollider.h"
#include "CarDriver/ICarDriver.h"

class ICarState;
class CarStateIdle;
class CarStateAccele;
class CarStateBrake;
class CarStateDrift;
class CheckedController;

namespace CAR{
	enum EnCarColor {
		enCar_Red,		//��
		enCar_Blue,		//��
		enCar_Num		//��ސ�
	};
}

class Car : public IGameObject
{
public:
	static const float MOVE_COEFFICIENT;				//���ԃx�[�X�̈ړ��Ɋ|����W���B

	enum EnDriverType {
		enTypePlayer,			//�v���C���[����B
		enTypeAI,				//AI����B
		enDriverTypeNum
	};

	enum EnCarState {
		enStateIdle,
		enStateAccele,
		enStateBrake,
		enStateDrift,
		enStateNum
	};
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
	friend class CarStateDrift;
public:
	/// <summary>
	/// �X�e�[�g��ύX����B
	/// </summary>
	/// <param name="state">�X�e�[�g�N���X�̃A�h���X�B</param>
	void ChangeState(ICarState* state);
	/// <summary>
	/// �Ԃ̃h���C�o�[��ݒ肷��B
	/// </summary>
	/// <param name="type">�h���C�o�[�̎�ށB</param>
	void SetCarDriver(EnDriverType type);
public:		//Set�֐��Ƃ�
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	void SetCarColor(const CAR::EnCarColor colorType)
	{
		m_carColor = colorType;
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
	//�S�[���ς݂��H
	bool IsGoal();

protected:		//�Ԃ̈ړ��Ȃǂ̏���
	/// <summary>
	/// �����x�𑬓x�ɉ��Z����B
	/// </summary>
	/// <param name="accele">�����x�B</param>
	void AddAcceleration(const float accele)
	{
		m_speed = min(MAX_SPEED, m_speed + (accele * MOVE_COEFFICIENT * GameTime().GetFrameDeltaTime()));
	}
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
	/// �L�����R�����g�����ړ��B
	/// </summary>
	void CharaConMove();

protected:
	static constexpr float MAX_SPEED = 6800.0f;

protected:	//�X�e�[�g��
	ICarState* m_currentState = nullptr;
	std::unique_ptr<CarStateIdle> m_stateIdle;
	std::unique_ptr<CarStateAccele> m_stateAccele;
	std::unique_ptr<CarStateBrake> m_stateBrake;
	std::unique_ptr<CarStateDrift> m_stateDrift;

	std::unique_ptr<ICarDriver> m_carDriver;
protected:
	CAR::EnCarColor m_carColor = CAR::enCar_Red;
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

