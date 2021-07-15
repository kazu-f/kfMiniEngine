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
	bool Start() override final;		//開始時処理。
	void OnDestroy() override final;	//削除時処理。
	void Update() override final;		//更新処理。
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
	prefab::ModelRender* m_treeModel = nullptr;			//木のモデル。
	std::vector<TreeObjData> m_treeObjDatas;			//木々のオブジェクトデータ。
	std::string m_filePath;								//木のモデルのファイルパス。
};

