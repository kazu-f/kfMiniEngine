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
	//���f����ǂݍ��ށB
	m_treeModel = NewGO<prefab::ModelRender>(0);

	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_filePath.c_str();
	//���f���������B
	m_treeModel->Init(
		modelInitData,
		nullptr,
		1,
		static_cast<int>(m_treeObjDatas.size())
	);
	//�����_�����O�t���O�B
	m_treeModel->SetShadowCasterFlag(true);
	m_treeModel->SetShadowReceiverFlag(true);
	m_treeModel->SetForwardRenderFlag(true);
	//�J�����O����B
	m_treeModel->SetCullingFar(20000.0f);
	return true;
}

void TreeInstancing::OnDestroy()
{
	DeleteGO(m_treeModel);
}

void TreeInstancing::Update()
{
	//�C���X�^���V���O�`��p�̃f�[�^���X�V�B
	if (m_treeObjDatas.size() > 1 && m_numInstancingNum < m_treeObjDatas.size()) {
		for (auto& objData : m_treeObjDatas) {
			m_treeModel->UpdateInstancingData(objData.pos, objData.rot, objData.scale);
			m_numInstancingNum++;
		}
	}
}
