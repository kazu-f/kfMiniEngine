#pragma once

class TreeInstancing : public IGameObject
{
	//�I�u�W�F�N�g�̃f�[�^�B
	struct TreeObjData {
		Vector3 pos;
		Quaternion rot;
		Vector3 scale;
	};
public:
	TreeInstancing();
	~TreeInstancing();
	bool Start() override final;		//�J�n�������B
	void OnDestroy() override final;	//�폜�������B
	void Update() override final;		//�X�V�����B
	//�I�u�W�F�N�g�f�[�^�ǉ��B
	void AddTreeObjData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		m_treeObjDatas.push_back({ pos,rot,scale });
	}
	//�t�@�C���p�X���w��B
	void SetTreeFilePath(const char* filePath)
	{
		m_filePath = filePath;
	}

private:
	prefab::ModelRender* m_treeModel = nullptr;			//�؂̃��f���B
	std::vector<TreeObjData> m_treeObjDatas;			//�؁X�̃I�u�W�F�N�g�f�[�^�B
	std::string m_filePath;								//�؂̃��f���̃t�@�C���p�X�B
};

