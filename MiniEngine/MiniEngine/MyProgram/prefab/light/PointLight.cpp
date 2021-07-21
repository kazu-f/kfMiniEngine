#include "stdafx.h"
#include "PointLight.h"

Engine::prefab::CPointLight::CPointLight()
{
}

Engine::prefab::CPointLight::~CPointLight()
{
}

bool Engine::prefab::CPointLight::StartSub()
{
	return true;
}

void Engine::prefab::CPointLight::Update()
{
	//カメラ空間での位置を計算。
	const Matrix& mView = MainCamera().GetViewMatrix();
	m_light.positionInView = m_light.position;
	mView.Apply(m_light.positionInView);
	//ライトのマテリアルIDグループを設定。
	m_light.lightingMaterialIDGroup = GetLightingMaterialIDGroup();
}

void Engine::prefab::CPointLight::SetAttn(const Vector3& attn)
{
	m_light.attn = attn;
	if (m_light.attn.y <= 0.0f)
	{
		ENGINE_MESSAGE_BOX("attn.yの値が不正です。\n"
			"この値は必ず0より大きい値を設定する必要があります。");
		m_light.attn.y = 0.001f;
	}
}
