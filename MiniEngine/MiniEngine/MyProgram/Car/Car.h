#pragma once
#include "CarMoveController.h"
class CheckedController;
class ICarDriver;

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
	enum EnDriverType {
		enTypePlayer,			//�v���C���[����B
		enTypeAI,				//AI����B
		enDriverTypeNum
	};

public:
	Car();
	~Car();
	bool Start() override;			//�����������B
	void Update() override;			//�X�V�����B
	void OnDestroy() override;		//�폜�������B
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
	//�Ԃ̃h���C�o��ݒ肷��B
	void SetCarDriver(EnDriverType type);

public:		//Get�֐��Ƃ�
	const Vector3& GetPosition()const
	{
		return m_position;
	}

	//�`�F�b�N�R���g���[���[���擾�B
	CheckedController* GetCheckedController()
	{
		return m_checkedCon.get();
	}
	//�S�[���ς݂��H
	bool IsGoal();

protected:
	CAR::EnCarColor m_carColor = CAR::enCar_Red;
	prefab::ModelRender* m_model = nullptr;

	std::unique_ptr<ICarDriver> m_carDriver;					//�Ԃ̑���̃h���C�o�[�B
	std::unique_ptr<CarMoveController> m_carMoveCon;			//�Ԃ̈ړ����Ǘ�����B
	std::unique_ptr<CheckedController> m_checkedCon;			//�`�F�b�N�|�C���g�ɊĎ������B

	Vector3 m_position = Vector3::Zero;				//���W�B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
};

