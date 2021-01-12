#pragma once

class CGameCamera;

class Car : public IGameObject
{
public:
	Car() {};
	~Car() {};
	bool Start() override;
	void Update() override;
	void OnDestroy() override;

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

	Vector3 m_position = Vector3::Zero;		//���W�B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
};

