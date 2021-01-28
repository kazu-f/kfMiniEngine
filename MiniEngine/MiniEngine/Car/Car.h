#pragma once

class CGameCamera;

class Car : public IGameObject
{
public:
	Car() {};
	~Car() {};
	bool Start() override;			//�����������B
	void Update() override;			//�X�V�����B
	void OnDestroy() override;		//�폜�������B

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

private:
	CGameCamera* m_camera = nullptr;
	prefab::ModelRender* m_model = nullptr;

	CCharacterController m_charaCon;		//�L�����R���B
	Vector3 m_position = Vector3::Zero;		//���W�B
	Vector3 m_moveSpeed = Vector3::Zero;	//�ړ����x�B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
};

