#pragma once


/// <summary>
/// �ϋq�̃N���X�B
/// </summary>
class Spectator : public IGameObject
{
	//�ϋq�̔z�u�f�[�^�B
	struct LowObjctData
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
	};

public:
	Spectator();
	~Spectator();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;

	/// <summary>
	/// �I�u�W�F�N�g�̏���ǉ��B
	/// </summary>
	void AddObjectData(Vector3& position, Quaternion& rotation, Vector3& scale)
	{
		m_objectsData.push_back({ position,rotation,scale });
	}
	/// <summary>
	/// ���f���̃t�@�C���p�X�w��B
	/// </summary>
	void SetModelFilePath(const char* filePath)
	{
		m_modelFilePath = filePath;
	}
	/// <summary>
	/// �A�j���[�V�����̃t�@�C���p�X�w��B
	/// </summary>
	void SetAnimFilePath(const char* filePath)
	{
		m_animFilePath = filePath;
	}
	/// <summary>
	/// �V���h�E�L���X�^�[�t���O�B
	/// </summary>
	void SetShadowCasterFlag(bool flag)
	{
		m_isShadowCaster = flag;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�t���O�B
	/// </summary>
	void SetShadowReceiverFlag(bool flag)
	{
		m_isShadowReceiverFlag = flag;
	}

private:
	prefab::ModelRender* m_model = nullptr;	//���f���B
	std::vector<LowObjctData> m_objectsData;	//�C���X�^���V���O�`��p�̃f�[�^�B

	const char* m_modelFilePath = nullptr;		//���f���̃t�@�C���p�X�B
	const char* m_animFilePath = nullptr;			//�A�j���[�V�����̃t�@�C���p�X�B
	bool m_isShadowCaster = false;			//�V���h�E�L���X�^�[�t���O�B
	bool m_isShadowReceiverFlag = false;	//�V���h�E���V�[�o�[�t���O�B
};

