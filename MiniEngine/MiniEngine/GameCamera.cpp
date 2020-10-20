#include "stdafx.h"
#include "GameCamera.h"

bool CGameCamera::Start()
{
	g_camera3D->SetPosition({ 0.0f, 50.0f, 100.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });

	const auto& targetPos = g_camera3D->GetTarget();
	Vector3 vCamera = g_camera3D->GetPosition() - targetPos;
	m_targetToPosLen = vCamera.Length();

	return true;
}

void CGameCamera::Update()
{
	//�K���ɃJ�������߂Â����艓��������B
	const auto& targetPos = g_camera3D->GetTarget();
	Vector3 vCamera = g_camera3D->GetPosition() - targetPos;
		
	m_targetToPosLen -= g_pad[0]->GetLStickYF();
	m_targetToPosLen = min(500.0f, max(50.0f, m_targetToPosLen));

	auto vDir = vCamera;
	vDir.Normalize();
	vCamera = vDir * m_targetToPosLen;
	g_camera3D->SetPosition(targetPos + vCamera);

	Quaternion qRot;
	//�J�������񂷁B
	qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
	auto camPos = g_camera3D->GetPosition();
	qRot.Apply(camPos);
	g_camera3D->SetPosition(camPos);
}
