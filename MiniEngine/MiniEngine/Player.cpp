#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{
	//3D���f�����쐬�B
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/unityChan2.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	initData.m_vsEntryPointFunc = "VSMainSkin";
	//�A�j���[�V�����̏������f�[�^�B
	AnimClipInitData animInitData[en_animNum];
	animInitData[en_animIdle].tkaFilePath = "Assets/animData/idle.tka";
	animInitData[en_animIdle].isLoop = true;
	animInitData[en_animRun].tkaFilePath = "Assets/animData/run.tka";
	animInitData[en_animRun].isLoop = true;

	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData, animInitData, en_animNum);

	Quaternion qRot;
	qRot.SetRotationDegX(90.0f);
	m_model->SetRotation(qRot);

	return true;
}

void Player::Update()
{
	if (g_pad[0]->IsPress(enButtonA)) {
		m_model->PlayAnimation(en_animIdle, 0.3f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_model->PlayAnimation(en_animRun, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonX)) {
		DeleteGO(this);
	}
}

void Player::OnDestroy()
{
	DeleteGO(m_model);
}