#pragma once
/// <summary>
/// �K�[�h���[���������N���X�B
/// </summary>

class Guardrail : public IGameObject
{
	struct LowObjctData
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
	};
public:
	Guardrail();
	~Guardrail();
	bool Start()override;
	void OnDestroy()override;
	void Update()override;
	/// <summary>
	/// �K�[�h���[����ǂݍ��ށB
	/// </summary>
	/// <param name="path">���x���̃t�@�C���p�X�B</param>
	/// <param name="parentPos">�e�̍��W�B</param>
	/// <param name="parentRot">�e�̉�]�B</param>
	void LoadGuardrail(const char* path, const Vector3& parentPos, const Quaternion& parentRot);

private:
	/// <summary>
	/// �{�b�N�X�R���C�_�[������B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	void CreateBoxCollider(const Vector3& pos, const Quaternion& rot);

private:
	using ColliderPtr = std::unique_ptr<CPhysicsStaticObject>;
	std::vector<ColliderPtr> m_colliders;
	const Vector3 BOX_SIZE = { 27.0f,75.0f,100.0f };

	prefab::ModelRender* m_model = nullptr;	//���f���B
	std::vector<LowObjctData> m_objectsData;	//�C���X�^���V���O�`��p�̃f�[�^�B
	int m_numRenderObjects = 0;					//�I�u�W�F�N�g�̐��B

	const char* m_modelFilePath = "Assets/modelData/Guardrail/SM_Guardrail.tkm";		//���f���̃t�@�C���p�X�B
	bool m_isShadowCaster = false;			//�V���h�E�L���X�^�[�t���O�B
	bool m_isShadowReceiverFlag = false;	//�V���h�E���V�[�o�[�t���O�B

};

