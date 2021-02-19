#pragma once
/// <summary>
/// ガードレールを扱うクラス。
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
	/// ガードレールを読み込む。
	/// </summary>
	/// <param name="path">レベルのファイルパス。</param>
	/// <param name="parentPos">親の座標。</param>
	/// <param name="parentRot">親の回転。</param>
	void LoadGuardrail(const char* path, const Vector3& parentPos, const Quaternion& parentRot);

private:
	/// <summary>
	/// ボックスコライダーをつける。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	void CreateBoxCollider(const Vector3& pos, const Quaternion& rot);

private:
	using ColliderPtr = std::unique_ptr<CPhysicsStaticObject>;
	std::vector<ColliderPtr> m_colliders;
	const Vector3 BOX_SIZE = { 27.0f,75.0f,100.0f };

	prefab::ModelRender* m_model = nullptr;	//モデル。
	std::vector<LowObjctData> m_objectsData;	//インスタンシング描画用のデータ。
	int m_numRenderObjects = 0;					//オブジェクトの数。

	const char* m_modelFilePath = "Assets/modelData/Guardrail/SM_Guardrail.tkm";		//モデルのファイルパス。
	bool m_isShadowCaster = false;			//シャドウキャスターフラグ。
	bool m_isShadowReceiverFlag = false;	//シャドウレシーバーフラグ。

};

