#include "stdafx.h"
#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//ライトを用意する。

	light.directionalLight[0].color.x = 0.5f;
	light.directionalLight[0].color.y = 0.5f;
	light.directionalLight[0].color.z = 0.5f;

	light.directionalLight[0].direction.x = 0.0f;
	light.directionalLight[0].direction.y = 0.0f;
	light.directionalLight[0].direction.z = -1.0f;



	light.ambinetLight.x = 0.2f;
	light.ambinetLight.y = 0.2f;
	light.ambinetLight.z = 0.2f;
	light.eyePos = g_camera3D->GetPosition();
	light.specPow = 5.0f;

	//3Dモデルを作成。



	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	initData.m_vsEntryPointFunc = "VSMainSkin";
	initData.m_expandConstantBuffer = &light;
	initData.m_expandConstantBufferSize = sizeof(light);

	robotModel.Init(initData);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 100.0f });
	Vector3 pos, scale;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	Quaternion qRot;
	qRot.SetRotationDegX(90.0f);

	robotModel.UpdateWorldMatrix(pos, qRot, scale);

	//アニメーションのためのロードを行う。
	if (m_skeleton.Init("Assets/modelData/unityChan.tks")) {
		if (m_skeleton.IsLoaded()) {
			//ボーンの構築。
			m_skeleton.BuildBoneMatrices();
			//モデルと関連付け。
			robotModel.BindSkeleton(m_skeleton);

			//アニメーションクリップのロード。
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

}

void GameScene::Release()
{
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

	for (auto& lig : light.directionalLight) {
		qRot.Apply(lig.direction);
	}

	if (g_pad[0]->IsPress(enButtonA)) {
		m_animation.Play(0, 0.6f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_animation.Play(1, 0.6f);
	}
	//カメラも回す。
	qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
	auto camPos = g_camera3D->GetPosition();
	qRot.Apply(camPos);
	g_camera3D->SetPosition(camPos);
	light.eyePos = g_camera3D->GetPosition();
	if (g_pad[0]->IsTrigger(enButtonA)) {
		isPBR = !isPBR;
	}
	if (m_animation.IsInited()) {
		//アニメーションを再生。
		m_animation.Progress(GameTime().GetFrameDeltaTime());
	}
	if (m_skeleton.IsInited()) {
		//スケルトンを更新。
		m_skeleton.Update(robotModel.GetWorldMatrix());
	}
}

void GameScene::Draw(RenderContext& rc)
{
	robotModel.Draw(rc);
}
