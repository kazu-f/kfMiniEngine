#include "stdafx.h"
#include "Spectator.h"

#define ON 1
#define OFF 0
#define ISBOX OFF			//�f�o�b�O�p�Ɋϋq�𔠂ɒu�������邩�B

namespace {
	const float CULL_FAR = 20000.0f;
}

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
		ENGINE_MESSAGE_BOX(
			"�ϋq�̃��f���̃t�@�C���p�X�����ݒ�B\n"
			"SetModelFilePath�֐��Ŏw�肵�Ă��������B\n"
		);
		return false;
	}
	if (m_animFilePath == nullptr)
	{
		ENGINE_MESSAGE_BOX(
			"�ϋq�̃A�j���[�V�����̃t�@�C���p�X�����ݒ�B\n"
			"SetAnimFilePath�֐��Ŏw�肵�Ă��������B\n"
		);
		return false;
	}
	if (m_objectsData.size() <= 1)
	{
		ENGINE_MESSAGE_BOX(
			"�ϋq�̃C���X�^���V���O�`��̃f�[�^������Ă��Ȃ��B\n"
			"�C���X�^���V���O�`��ɂ�2�ȏ�K�v�ł��B\n"
		);
		return false;
	}

	//���f���������B
	m_model = NewGO<prefab::ModelRender>(10);
	ModelInitData modelInitData;
#if ISBOX
	modelInitData.m_tkmFilePath = "Assets/modelData/testbg/testBox.tkm";	//�f�o�b�O�\���p�̔��̃��f���B
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";					//���f���V�F�[�_�[�B
	modelInitData.m_vsEntryPointFunc = "VSMainInstancing";					//�C���X�^���V���O�`��B

	//�������B
	m_model->Init(
		modelInitData,
		nullptr,
		0,
		static_cast<int>(m_objectsData.size())
	);
#else
	modelInitData.m_tkmFilePath = m_modelFilePath;
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
#endif

	//�e�̐ݒ�B
	m_model->SetShadowCasterFlag(m_isShadowCaster);
	m_model->SetShadowReceiverFlag(false);
	m_model->SetCullingFar(CULL_FAR);

	return true;
}

void Spectator::Update()
{
	//���Z�b�g�B
	m_model->BeginUpdateInstancingData();
	//�C���X�^���V���O�`��p�̃f�[�^���X�V�B
	for (auto& objData : m_objectsData) {
		m_model->UpdateInstancingData(objData.position, objData.rotation, objData.scale,true);
	}
}

void Spectator::OnDestroy()
{
	//�폜�B
	DeleteGO(m_model);
}
