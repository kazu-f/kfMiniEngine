#include "stdafx.h"
#include "TreeInstancing.h"

TreeInstancing::TreeInstancing()
{
}

TreeInstancing::~TreeInstancing()
{
}

bool TreeInstancing::Start()
{
	m_treeModel = NewGO<prefab::ModelRender>(0);

	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_filePath.c_str();
	//�������B
	m_treeModel->Init(
		modelInitData,
		nullptr,
		1,
		static_cast<int>(m_treeObjDatas.size())
	);

	m_treeModel->SetShadowCasterFlag(true);
	m_treeModel->SetShadowReceiverFlag(true);
	m_treeModel->SetForwardRenderFlag(true);

	m_treeModel->SetCullingFar(20000.0f);
	return true;
}

void TreeInstancing::Update()
{
	//�C���X�^���V���O�`��p�̃f�[�^���X�V�B
	m_treeModel->BeginUpdateInstancingData();
		for (auto& objData : m_treeObjDatas) {
			m_treeModel->UpdateInstancingData(objData.pos, objData.rot, objData.scale);
		}
}
