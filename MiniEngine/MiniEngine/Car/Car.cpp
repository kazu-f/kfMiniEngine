#include "stdafx.h"
#include "Car.h"
#include "GameCamera.h"

namespace {
	const float MOVE_SPEED = 4000.0f;
}


bool Car::Start()
{
	//3Dƒ‚ƒfƒ‹‚ğì¬B
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/Car/Car.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData);
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_camera->SetTarget(m_position);

	m_model->SetShadowCasterFlag(true);

	return true;
}

void Car::Update()
{
	const float PadX = g_pad[0]->GetLStickXF();
	const float PadY = g_pad[0]->GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	m_position.x += PadX * MOVE_SPEED * DeltaTime;
	m_position.z += PadY * MOVE_SPEED * DeltaTime;

	m_model->SetPosition(m_position);
	m_camera->SetTarget(m_position);

	//if (g_pad[0]->IsTrigger(enButtonX)) {
	//	DeleteGO(this);
	//}
}

void Car::OnDestroy()
{
	DeleteGO(m_model);
}
