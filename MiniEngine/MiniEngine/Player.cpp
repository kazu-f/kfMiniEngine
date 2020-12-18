#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{
	////3Dモデルを作成。
	//ModelInitData initData;
	//initData.m_tkmFilePath = "Assets/modelData/unityChan2.tkm";
	//initData.m_fxFilePath = "Assets/shader/model.fx";
	//initData.m_vsEntryPointFunc = "VSMainSkin";
	////アニメーションの初期化データ。
	//AnimClipInitData animInitData[en_animNum];
	//animInitData[en_animIdle].tkaFilePath = "Assets/animData/idle.tka";
	//animInitData[en_animIdle].isLoop = true;
	//animInitData[en_animRun].tkaFilePath = "Assets/animData/run.tka";
	//animInitData[en_animRun].isLoop = true;
	//Quaternion qRot;
	//qRot.SetRotationDegX(90.0f);
	//m_model->SetRotation(qRot);

	//ModelInitData initData;
	//initData.m_tkmFilePath = "Assets/modelData/Human/suitWoman/suitWoman.tkm";
	//initData.m_fxFilePath = "Assets/shader/model.fx";
	//initData.m_vsEntryPointFunc = "VSMainSkin";
	////アニメーションの初期化データ。
	//AnimClipInitData animInitData[en_animNum];
	//animInitData[en_animIdle].tkaFilePath = "Assets/animData/Human/Clap.tka";
	//animInitData[en_animIdle].isLoop = true;
	//animInitData[en_animRun].tkaFilePath = "Assets/animData/Human/Cheering.tka";
	//animInitData[en_animRun].isLoop = true;

	//ModelInitData initData;
	//initData.m_tkmFilePath = "Assets/modelData/Human/shirtMale/shirtMale.tkm";
	//initData.m_fxFilePath = "Assets/shader/model.fx";
	//initData.m_vsEntryPointFunc = "VSMainSkin";
	////アニメーションの初期化データ。
	//AnimClipInitData animInitData[en_animNum];
	//animInitData[en_animIdle].tkaFilePath = "Assets/animData/Human/StandingClap.tka";
	//animInitData[en_animIdle].isLoop = true;
	//animInitData[en_animRun].tkaFilePath = "Assets/animData/Human/Cheering.tka";
	//animInitData[en_animRun].isLoop = true;

	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/Human/suitMale/suitMale.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	initData.m_vsEntryPointFunc = "VSMainSkin";
	//アニメーションの初期化データ。
	AnimClipInitData animInitData[en_animNum];
	animInitData[en_animIdle].tkaFilePath = "Assets/animData/Human/walk.tka";
	animInitData[en_animIdle].isLoop = true;
	animInitData[en_animRun].tkaFilePath = "Assets/animData/Human/Cheering.tka";
	animInitData[en_animRun].isLoop = true;

	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData, animInitData, en_animNum);
	//m_model->Init(initData);
	m_model->SetPosition(m_position);

	m_model->SetShadowCasterFlag(true);

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

	m_position += m_model->GetFootstepMove();
	m_model->SetPosition(m_position);
}

void Player::OnDestroy()
{
	DeleteGO(m_model);
}
