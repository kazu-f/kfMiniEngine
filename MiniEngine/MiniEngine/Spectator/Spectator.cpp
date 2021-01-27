#include "stdafx.h"
#include "Spectator.h"

Spectator::Spectator()
{
}

Spectator::~Spectator()
{
}

bool Spectator::Start()
{
	//ファイルパス未設定。
	if (m_modelFilePath == nullptr)
	{
		return false;
	}
	if (m_animFilePath == nullptr)
	{
		return false;
	}

	//モデル初期化。
	m_model = NewGO<prefab::ModelRender>(0);
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_modelFilePath;
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	modelInitData.m_vsEntryPointFunc = "VSMainSkinInstancing";
	modelInitData.m_modelUpAxis = enModelUpAxis_Z;


	//アニメーション情報。
	AnimClipInitData animInitData[1];
	animInitData[0].tkaFilePath = m_animFilePath;
	animInitData[0].isLoop = true;
	//初期化。
	m_model->Init(
		modelInitData, 
		animInitData, 
		1,
		static_cast<int>(m_objectsData.size())
	);

	//影の設定。
	m_model->SetShadowCasterFlag(m_isShadowCaster);
	m_model->SetShadowReceiverFlag(false);

	return true;
}

void Spectator::Update()
{
	//インスタンシング描画用のデータを更新。
	if (m_numRenderObjects < m_objectsData.size()) {
		for (auto& objData : m_objectsData) {
			m_model->UpdateInstancingData(objData.position, objData.rotation, objData.scale);
			m_numRenderObjects++;
		}
	}
}

void Spectator::OnDestroy()
{
	//削除。
	DeleteGO(m_model);
}
