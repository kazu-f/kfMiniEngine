#include "stdafx.h"
#include "SceneLight.h"
#include "prefab/light/DirectionLight.h"

SceneLight::SceneLight()
{
}

SceneLight::~SceneLight()
{
}

bool SceneLight::Start()
{
	//ライトを用意する。
	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);

	Vector4 color = Vector4::Gray * 3.0f;
	m_lightDir = { 0.0f,-1.0f,-1.0f };
	//color = { 1.0f,1.0f,0.0f,1.0f };
	light->SetColor(color);
	light->SetDirection(m_lightDir);

	m_lightArray.push_back(light);

	//prefab::CDirectionLight* light2 = NewGO<prefab::CDirectionLight>(0);

	//m_lightDir = { 0.0f,0.0f,1.0f };
	////color = { 0.0f,1.0f,1.0f,1.0f };
	//light2->SetColor(color);
	//light2->SetDirection(m_lightDir);

	//m_lightArray.push_back(light2);

	//prefab::CDirectionLight* light3 = NewGO<prefab::CDirectionLight>(0);

	//m_lightDir = { 0.0f,-1.0f,0.0f };
	////color = { 0.0f,1.0f,1.0f,1.0f };
	//light3->SetColor(color);
	//light3->SetDirection(m_lightDir);

	//m_lightArray.push_back(light3);

	return true;
}
