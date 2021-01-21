#pragma once



class Spectator : public IGameObject
{
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
	/// オブジェクトの情報を追加。
	/// </summary>
	void AddObjectData(Vector3& position, Quaternion& rotation, Vector3& scale)
	{
		m_objectsData.push_back({ position,rotation,scale });
	}
	/// <summary>
	/// モデルのファイルパス指定。
	/// </summary>
	void SetModelFilePath(const char* filePath)
	{
		m_modelFilePath = filePath;
	}
	/// <summary>
	/// アニメーションのファイルパス指定。
	/// </summary>
	void SetAnimFilePath(const char* filePath)
	{
		m_modelFilePath = filePath;
	}
	/// <summary>
	/// シャドウキャスターフラグ。
	/// </summary>
	void SetShadowCasterFlag(bool flag)
	{
		m_isShadowCaster = flag;
	}
	/// <summary>
	/// シャドウレシーバーフラグ。
	/// </summary>
	void SetShadowReceiverFlag(bool flag)
	{
		m_isShadowReceiverFlag = flag;
	}

private:
	prefab::ModelRender* m_model = nullptr;	//モデル。
	prefab::ModelRender* m_testModel = nullptr;
	std::vector<LowObjctData> m_objectsData;	//インスタンシング描画用のデータ。
	int m_numRenderObjects = 0;					//オブジェクトの数。

	const char* m_modelFilePath = nullptr;		//モデルのファイルパス。
	const char* m_animFilePath = nullptr;			//アニメーションのファイルパス。
	bool m_isShadowCaster = false;			//シャドウキャスターフラグ。
	bool m_isShadowReceiverFlag = false;	//シャドウレシーバーフラグ。
};

