#include "stdafx.h"
#include "SceneLight.h"

namespace {
	const float AMBIENT_LIGHT_POWER = 1.0f;
	const Vector3 AMBIENT_LIGHT = { AMBIENT_LIGHT_POWER,AMBIENT_LIGHT_POWER,AMBIENT_LIGHT_POWER };
	const Vector3 SHADOW_DIRECTION = { 0.0f,-1.0f,0.0f };
	const Vector4 DIRLIGHT_POWER = Vector4::White * 3.0f;
}

SceneLight::SceneLight()
{
}

SceneLight::~SceneLight()
{
}

void SceneLight::OnDestroy()
{
	//ライトの解放。
	for (auto* lig : m_lightArray)
	{
		DeleteGO(lig);
	}
}

bool SceneLight::Start()
{
	//ライトを用意する。
	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);

	//Vector4 color = Vector4::Gray * 3.0f;
	Vector4 color = DIRLIGHT_POWER;
	m_lightDir = { 0.0f,-1.0f,-1.0f };
	//color = { 1.0f,1.0f,0.0f,1.0f };
	light->SetColor(color);
	light->SetDirection(m_lightDir);

	m_lightArray.push_back(light);

	prefab::CDirectionLight* light2 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { -1.0f,-1.0f,0.0f };
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

	//環境光の設定。
	GraphicsEngine()->GetLightManager()->SetAmbientLight(AMBIENT_LIGHT);

	//影の落ちる向きを設定。
	GraphicsEngine()->GetShadowMap()->SetLightDirection(SHADOW_DIRECTION);

	return true;
}

