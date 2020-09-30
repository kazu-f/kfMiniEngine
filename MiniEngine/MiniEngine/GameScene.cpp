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
	initData.m_fxFilePath = "Assets/shader/NoAnimModel_PBR.fx";
	initData.m_expandConstantBuffer = &light;
	initData.m_expandConstantBufferSize = sizeof(light);

	robotPBRModel.Init(initData);
	initData.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	robotModel.Init(initData);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 100.0f });
	Vector3 pos, scale;


	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	robotPBRModel.UpdateWorldMatrix(pos, g_quatIdentity, scale);
	robotModel.UpdateWorldMatrix(pos, g_quatIdentity, scale);
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

	//カメラも回す。
	qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
	auto camPos = g_camera3D->GetPosition();
	qRot.Apply(camPos);
	g_camera3D->SetPosition(camPos);
	light.eyePos = g_camera3D->GetPosition();
	if (g_pad[0]->IsTrigger(enButtonA)) {
		isPBR = !isPBR;
	}
}

void GameScene::Draw(RenderContext& rc)
{
	if (isPBR) {
		//3Dモデルを表示する。
		robotPBRModel.Draw(rc);
	}
	else {
		robotModel.Draw(rc);
	}
}
