#include "stdafx.h"
#include "SceneLight.h"

namespace {
	const float AMBIENT_LIGHT_POWER = 0.8f;
	const Vector3 AMBIENT_LIGHT = { AMBIENT_LIGHT_POWER,AMBIENT_LIGHT_POWER,AMBIENT_LIGHT_POWER };
	const Vector3 SHADOW_DIRECTION = { 0.173f,-0.984f,0.173f };
	const float DIRLIGHT_POWER = 5.5f;
	const Vector4 DIRLIGHT_COLOR = { DIRLIGHT_POWER,DIRLIGHT_POWER,DIRLIGHT_POWER,1.0f };
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
	Vector4 color = DIRLIGHT_COLOR;
	m_lightDir = SHADOW_DIRECTION;
	m_lightDir.Set(1.0f, -1.0f, 0.0f);
	m_lightDir.Normalize();
	//color = { 1.0f,1.0f,0.0f,1.0f };
	light->SetColor(color);
	light->SetDirection(m_lightDir);

	m_lightArray.push_back(light);

	prefab::CDirectionLight* light2 = NewGO<prefab::CDirectionLight>(0);

	color = { 2.3f, 2.3f, 2.3f ,1.0f};
	m_lightDir = { -1.0f,-1.0f,0.0f };
	//color = { 0.0f,1.0f,1.0f,1.0f };
	light2->SetColor(color);
	light2->SetDirection(m_lightDir);

	m_lightArray.push_back(light2);

	prefab::CDirectionLight* light3 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 0.0f,-1.0f,1.0f };
	//color = { 0.0f,1.0f,1.0f,1.0f };
	light3->SetColor(color);
	light3->SetDirection(m_lightDir);

	m_lightArray.push_back(light3);

	prefab::CDirectionLight* light4 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 1.0f,-1.0f,0.0f };
	light4->SetColor(color);
	light4->SetDirection(m_lightDir);

	m_lightArray.push_back(light4);

	prefab::CDirectionLight* light5 = NewGO<prefab::CDirectionLight>(0);

	m_lightDir = { 0.0f,-1.0f,0.0f };
	light5->SetColor(color);
	light5->SetDirection(m_lightDir);

	m_lightArray.push_back(light5);

	//環境光の設定。
	GraphicsEngine()->GetLightManager()->SetAmbientLight(AMBIENT_LIGHT);

	//影の落ちる向きを設定。
	GraphicsEngine()->GetShadowMap()->SetLightDirection(SHADOW_DIRECTION);

	return true;
}

