#include "stdafx.h"
#include "GameCamera.h"

namespace {
	const Vector3 CAMERA_DISTANCE = { 0.0f,200.0f,500.0f };
}

bool CGameCamera::Start()
{
	//g_camera3D->SetPosition({ 0.0f, 150.0f, 100.0f });
	//g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });

	//const auto& targetPos = g_camera3D->GetTarget();
	//Vector3 vCamera = g_camera3D->GetPosition() - targetPos;
	//m_targetToPosLen = vCamera.Length();

	g_camera3D->SetFar(10000.0f);

	Vector3 cameraPos = m_target + CAMERA_DISTANCE;

	g_camera3D->SetPosition(cameraPos);
	g_camera3D->SetTarget(m_target);

	return true;
}

void CGameCamera::Update()
{
	////適当にカメラを近づけたり遠ざけたり。
	//const auto& targetPos = g_camera3D->GetTarget();
	//Vector3 vCamera = g_camera3D->GetPosition() - targetPos;
	//const float deltaTime = static_cast<float>(GameTime().GetFrameDeltaTime());
	//const float cameraSpeed = 100.0f;
	//	
	//m_targetToPosLen -= g_pad[0]->GetLStickYF() * deltaTime * cameraSpeed;
	//m_targetToPosLen = min(500.0f, max(50.0f, m_targetToPosLen));

	//auto vDir = vCamera;
	//vDir.Normalize();
	//vCamera = vDir * m_targetToPosLen;
	//g_camera3D->SetPosition(targetPos + vCamera);

	//Quaternion qRot;
	////カメラを回す。
	//qRot.SetRotationDegY(g_pad[0]->GetLStickXF() * deltaTime * cameraSpeed);
	//auto camPos = g_camera3D->GetPosition();
	//qRot.Apply(camPos);
	//g_camera3D->SetPosition(camPos);

	Vector3 cameraPos = m_target + CAMERA_DISTANCE;

	g_camera3D->SetPosition(cameraPos);
	g_camera3D->SetTarget(m_target);
}
