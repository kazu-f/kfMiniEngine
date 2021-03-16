#include "stdafx.h"
#include "GameCamera.h"

namespace {
	const Vector3 CAMERA_DISTANCE = { 0.0f,200.0f,500.0f };
	const float CAMERAMOV_SPEED = 100.0f;
	const float CAMERAROT_SPEED = 20.0f;
}

bool CGameCamera::Start()
{
	//g_camera3D->SetPosition({ 0.0f, 150.0f, 100.0f });
	//g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });

	const auto& targetPos = MainCamera().GetTarget();
	Vector3 vCamera = MainCamera().GetPosition() - targetPos;
	m_targetToPosLen = vCamera.Length();

	MainCamera().SetFar(10000.0f);

	Vector3 cameraPos = m_target + CAMERA_DISTANCE;

	MainCamera().SetPosition(cameraPos);
	MainCamera().SetTarget(m_target);

	return true;
}

void CGameCamera::Update()
{
	//適当にカメラを近づけたり遠ざけたり。
	const auto& targetPos = MainCamera().GetTarget();
	Vector3 vCamera = MainCamera().GetPosition() - targetPos;
	const float deltaTime = static_cast<float>(GameTime().GetFrameDeltaTime());
		
	m_targetToPosLen -= g_pad[0]->GetRStickYF() * deltaTime * CAMERAMOV_SPEED;
	m_targetToPosLen = min(500.0f, max(50.0f, m_targetToPosLen));

	auto vDir = vCamera;
	vDir.Normalize();
	vCamera = vDir * m_targetToPosLen;
	MainCamera().SetPosition(targetPos + vCamera);

	Quaternion qRot;
	//カメラを回す。
	qRot.SetRotationDegY(g_pad[0]->GetRStickXF() * GameTime().GetFrameDeltaTime() * CAMERAROT_SPEED);
	auto camPos = MainCamera().GetPosition() - MainCamera().GetTarget();
	qRot.Apply(camPos);

	Vector3 cameraPos = m_target + camPos;

	MainCamera().SetPosition(cameraPos);
	MainCamera().SetTarget(m_target);
}
