#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

bool BackGround::Start()
{
	//���f���̏��������s���B�B
	m_model = NewGO<prefab::ModelRender>(0);
	ModelInitData initData;
	initData.m_fxFilePath = "Assets/shader/model.fx";
	initData.m_tkmFilePath = "Assets/modelData/Road/Ground.tkm";
	m_model->Init(initData);
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetScale(m_scale);
	m_model->SetShadowReceiverFlag(true);

	//�n�ʂ̓����蔻��p�̃��b�V����ǂݍ��ށB
	m_mesh.Load("Assets/modelData/Road/GroundMesh.tkm");
	//�ÓI�����I�u�W�F�N�g���쐬����B
	m_phyStaticObj.CreateMesh(m_position, m_rotation, m_mesh);

	return true;
}

void BackGround::Update()
{

}

void BackGround::OnDestroy()
{
	DeleteGO(m_model);
}
