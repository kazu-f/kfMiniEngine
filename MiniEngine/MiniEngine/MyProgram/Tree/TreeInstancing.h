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
	void AddTreeObjData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		m_treeObjDatas.push_back({ pos,rot,scale });
	}

private:
	prefab::ModelRender* m_treeModel = nullptr;
	std::vector<TreeObjData> m_treeObjDatas;
};

