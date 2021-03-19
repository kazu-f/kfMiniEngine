#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	/// <summary>
	/// ���W�A��]�A�X�P�[����ݒ肷��B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="scale">�X�P�[���B</param>
	void InitData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		m_position = pos;
		m_rotation = rot;
		m_scale = scale;
	}

private:
	prefab::ModelRender* m_model = nullptr;		//���f���\���B
	TkmFile m_mesh;								//�����蔻��p�̃��b�V���B
	CPhysicsStaticObject m_phyStaticObj;		//�ÓI�����I�u�W�F�N�g�B
	Vector3 m_position = Vector3::Zero;			//���W�B
	Quaternion m_rotation = Quaternion::Identity;	//��]�B
	Vector3 m_scale = Vector3::One;				//�X�P�[���B
};

