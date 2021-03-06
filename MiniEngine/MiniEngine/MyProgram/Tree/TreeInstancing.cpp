#include "stdafx.h"
#include "TreeInstancing.h"

namespace {
	const float CULL_FAR = 50000.0f;
}

TreeInstancing::TreeInstancing()
{
}

TreeInstancing::~TreeInstancing()
{
}

bool TreeInstancing::Start()
{
	//モデルを読み込む。
	m_treeModel = NewGO<prefab::ModelRender>(0);

	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_filePath.c_str();
	//モデル初期化。
	m_treeModel->Init(
		modelInitData,
		nullptr,
		1,
		static_cast<int>(m_treeObjDatas.size())
	);
	//レンダリングフラグ。
	m_treeModel->SetShadowCasterFlag(true);
	m_treeModel->SetShadowReceiverFlag(true);
	m_treeModel->SetForwardRenderFlag(true);
	//カリングする。
	m_treeModel->SetCullingFar(CULL_FAR);
	return true;
}

void TreeInstancing::OnDestroy()
{
	DeleteGO(m_treeModel);
}

void TreeInstancing::Update()
{
	//インスタンシング描画用のデータを更新。
	m_treeModel->BeginUpdateInstancingData();
	for (auto& objData : m_treeObjDatas) {
		m_treeModel->UpdateInstancingData(objData.pos, objData.rot, objData.scale,true);
	}
}
