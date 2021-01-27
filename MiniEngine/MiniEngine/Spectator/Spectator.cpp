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
	//�t�@�C���p�X���ݒ�B
	if (m_modelFilePath == nullptr)
	{
		return false;
	}
	if (m_animFilePath == nullptr)
	{
		return false;
	}

	//���f���������B
	m_model = NewGO<prefab::ModelRender>(0);
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_modelFilePath;
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	modelInitData.m_vsEntryPointFunc = "VSMainSkinInstancing";
	modelInitData.m_modelUpAxis = enModelUpAxis_Z;


	//�A�j���[�V�������B
	AnimClipInitData animInitData[1];
	animInitData[0].tkaFilePath = m_animFilePath;
	animInitData[0].isLoop = true;
	//�������B
	m_model->Init(
		modelInitData, 
		animInitData, 
		1,
		static_cast<int>(m_objectsData.size())
	);

	//�e�̐ݒ�B
	m_model->SetShadowCasterFlag(m_isShadowCaster);
	m_model->SetShadowReceiverFlag(false);

	return true;
}

void Spectator::Update()
{
	//�C���X�^���V���O�`��p�̃f�[�^���X�V�B
	if (m_numRenderObjects < m_objectsData.size()) {
		for (auto& objData : m_objectsData) {
			m_model->UpdateInstancingData(objData.position, objData.rotation, objData.scale);
			m_numRenderObjects++;
		}
	}
}

void Spectator::OnDestroy()
{
	//�폜�B
	DeleteGO(m_model);
}
