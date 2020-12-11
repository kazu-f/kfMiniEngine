#include "stdafx.h"
#include "Car.h"

bool Car::Start()
{
	//3Dƒ‚ƒfƒ‹‚ğì¬B
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/Car/Car.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData);
	m_model->SetPosition(m_position);

	m_model->SetShadowCasterFlag(true);

	return true;
}

void Car::Update()
{
	if (g_pad[0]->IsTrigger(enButtonX)) {
		DeleteGO(this);
	}
}

void Car::OnDestroy()
{
	DeleteGO(m_model);
}
