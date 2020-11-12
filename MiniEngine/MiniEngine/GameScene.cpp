#include "stdafx.h"
#include "GameScene.h"
#include "prefab/light/DirectionLight.h"
#include "GameCamera.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::Start()
{
	//���C�g��p�ӂ���B
	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);

	Vector4 color = Vector4::Gray * 1.0f;
	m_lightDir = { 0.0f,0.0f,-1.0f };
	//color = { 1.0f,1.0f,0.0f,1.0f };
	light->SetColor(color);
	light->SetDirection(m_lightDir);

	m_lightArray.push_back(light);

	prefab::CDirectionLight* light2 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 0.0f,0.0f,1.0f };
	//color = { 0.0f,1.0f,1.0f,1.0f };
	light2->SetColor(color);
	light2->SetDirection(m_lightDir);

	m_lightArray.push_back(light2);

	prefab::CDirectionLight* light3 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 0.0f,-1.0f,0.0f };
	//color = { 0.0f,1.0f,1.0f,1.0f };
	light3->SetColor(color);
	light3->SetDirection(m_lightDir);

	m_lightArray.push_back(light3);


	//3D���f�����쐬�B


	initData.m_tkmFilePath = "Assets/modelData/unityChan2.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	initData.m_vsEntryPointFunc = "VSMainSkin";
	//initData.m_expandConstantBuffer = &light;
	//initData.m_expandConstantBufferSize = sizeof(light);

	m_model.Init(initData);
	
	
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_tkmFilePath = "Assets/modelData/testbg/bg.tkm";
	m_testBox.Init(initData);

	m_camera = NewGO<CGameCamera>(0);

	Vector3 pos, scale;
	pos = { 0.0f,0.0f,0.0f };

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	Quaternion qRot;
	qRot.SetRotationDegX(90.0f);

	m_model.UpdateWorldMatrix(pos, qRot, scale);

	m_testBox.UpdateWorldMatrix({ 10.0f,0.0f,0.0f }, Quaternion::Identity, scale);
	m_testBox.SetShadowReceiverFlag(true);

	//�A�j���[�V�����̂��߂̃��[�h���s���B
	if (m_skeleton.Init("Assets/modelData/unityChan2.tks")) {
		if (m_skeleton.IsLoaded()) {
			//�{�[���̍\�z�B
			m_skeleton.BuildBoneMatrices();
			//���f���Ɗ֘A�t���B
			m_model.BindSkeleton(m_skeleton);

			//�A�j���[�V�����N���b�v�̃��[�h�B
			auto animClip = std::make_unique<CAnimationClip>();
			animClip->Load("Assets/animData/idle.tka");
			m_animationClips.push_back(move(animClip));
			auto animClipRun = std::make_unique<CAnimationClip>();
			animClipRun->Load("Assets/animData/run.tka");
			m_animationClips.push_back(move(animClipRun));

			for (auto& animClip : m_animationClips) {
				animClip->SetLoopFlag(true);
				animClip->BuildKeyFramesAndAnimationEvents();
			}

			m_animation.Init(m_skeleton, m_animationClips);
		}
	}

	return true;
}

void GameScene::Release()
{
}

void GameScene::PreUpdate()
{
	//�V���h�E�L���X�^�[�o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
}

void GameScene::Update()
{
	Quaternion qRot;
	if (g_pad[0]->IsPress(enButtonRight)) {
		qRot.SetRotationDegY(1.0f);
	}
	else if (g_pad[0]->IsPress(enButtonLeft)) {
		qRot.SetRotationDegY(-1.0f);
	}

	if (g_pad[0]->IsPress(enButtonA)) {
		m_animation.Play(0, 0.6f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_animation.Play(1, 0.6f);
	}

	////�����̕ω���������B
	//m_lightPow += g_pad[0]->GetLStickYF() * 0.02f;
	//m_lightPow = min(1.0f, max(0.0f, m_lightPow));
	//g_graphicsEngine->GetLightManager()->SetAmbientLight({ m_lightPow,m_lightPow,m_lightPow });


	if (m_animation.IsInited()) {
		//�A�j���[�V�������Đ��B
		m_animation.Progress(GameTime().GetFrameDeltaTime());
	}
	if (m_skeleton.IsInited()) {
		//�X�P���g�����X�V�B
		m_skeleton.Update(m_model.GetWorldMatrix());
	}
}

void GameScene::PreRender(RenderContext& rc)
{
	m_model.Draw(rc);
	m_testBox.Draw(rc);
}
