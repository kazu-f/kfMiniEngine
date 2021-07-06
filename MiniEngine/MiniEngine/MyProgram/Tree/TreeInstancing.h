#pragma once

class TreeInstancing : public IGameObject
{
	//オブジェクトのデータ。
	struct TreeObjData {
		Vector3 pos;
		Quaternion rot;
		Vector3 scale;
	};
public:
	TreeInstancing();
	~TreeInstancing();
	bool Start() override final;
	void Update() override final;
	//オブジェクトデータ追加。
	void AddTreeObjData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		m_treeObjDatas.push_back({ pos,rot,scale });
	}
	//ファイルパスを指定。
	void SetTreeFilePath(const char* filePath)
	{
		m_filePath = filePath;
	}

private:
	prefab::ModelRender* m_treeModel = nullptr;
	std::vector<TreeObjData> m_treeObjDatas;
	std::string m_filePath;
	int m_numRenderObjects = 0;
};

