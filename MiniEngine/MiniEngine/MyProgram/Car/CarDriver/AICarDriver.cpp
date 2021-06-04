#include "stdafx.h"
#include "AICarDriver.h"
#include "CheckPoint/CheckPointManager.h"
#include "Car/Car.h"

namespace {
	//�n���h���ʂ���`�ϊ����邽�߂̕ϐ��B
	const float HANDLE_FROM_ANGLE_START = 0.8f;
	const float HANDLE_FROM_ANGLE_END = 1.0f;
	const float HANDLE_FROM_DIST_START = 600.0f;
	const float HANDLE_FROM_DIST_END = 100.0f;
}

void AICarDriver::Update()
{
	//�h���C�o�[�̓��͂����Z�b�g�B
	ResetDriverInput();

	CheckPointManager* checkMgr = CheckPointManager::GetInstance();
	if (checkMgr == nullptr) return;

	//���݂̍��W���擾�B
	auto checkCon = m_car->GetCheckedController();
	Vector3 pos = checkCon->GetPosition();
	pos.y = 0.0f;

	//�`�F�b�N�|�C���g�̍��W���擾�B
	int currentNo = checkCon->GetCheckPointNo();
	CheckPointBase* currentCheckPoint = checkMgr->GetCheckPoint(currentNo);
	Vector3 currentCheckPointPos = currentCheckPoint->GetPosition();
	Vector3 backCheckPointPos = currentCheckPoint->GetBackCheckPoint()->GetPosition();
	
	//�`�F�b�N�|�C���g���q����������̋����Ńn���h���ʂ����߂�B
	//�`�F�b�N�|�C���g���q�������x�N�g�������߂�B
	Vector3 pointToPointDir = currentCheckPointPos - backCheckPointPos;
	pointToPointDir.y = 0.0f;
	float checkPointLen = pointToPointDir.Length();
	pointToPointDir.Normalize();
	//��O�̃`�F�b�N�|�C���g����Ԃ܂ł̃x�N�g���B
	Vector3 backCheckPointToCar = pos - backCheckPointPos;
	backCheckPointToCar.y = 0.0f;

	//��������̋��������߂Ă����B
	float backToCarDist = pointToPointDir.Dot(backCheckPointToCar);
	Vector3 onLineCarPos = backCheckPointPos + (pointToPointDir * backToCarDist);		//������̈ʒu�B
	Vector3 lineToCar = pos - onLineCarPos;
	float onLineBackToCarDist = lineToCar.Length();
	float distHandle = (HANDLE_FROM_DIST_END - onLineBackToCarDist) / (HANDLE_FROM_DIST_END - HANDLE_FROM_DIST_START);
	distHandle = max(0.0f, distHandle);

	backCheckPointToCar.Normalize();
	Vector3 pointToPointRight = pointToPointDir;
	Quaternion qRot;
	qRot.SetRotationDegY(90.0f);
	qRot.Apply(pointToPointRight);
	pointToPointRight.Normalize();

	if (backCheckPointToCar.Dot(pointToPointRight) > 0.0f)
	{
		distHandle *= -1.0f;
	}


	//�Ԃ̕����B
	Vector3 carForward = checkCon->GetForward();
	Vector3 carRight = checkCon->GetRight();
	//���̃`�F�b�N�|�C���g�ւ̊p�x����n���h���ʂ����߂�B
	//�Ԃ���`�F�b�N�|�C���g�ւ̃x�N�g���B
	Vector3 CarToCheckPoint = currentCheckPointPos - pos;
	CarToCheckPoint.y = 0.0f;		//XZ���ʉ��B
	CarToCheckPoint.Normalize();

	//�n���h����؂�ׂ������𒲂ׂ�B
	float dirPow = backToCarDist / checkPointLen;
	dirPow = max(0.0f, min(1.0f, dirPow));
	//Vector3 handleDir = pointToPointDir * (1.0f - dirPow) + currentCheckPoint->GetDirection() * dirPow;
	Vector3 handleDir = currentCheckPoint->GetBackCheckPoint()->GetDirection() * (1.0f - dirPow) + currentCheckPoint->GetDirection() * dirPow;
	//Vector3 handleDir = currentCheckPoint->GetBackCheckPoint()->GetDirection() * (1.0f - dirPow) + CarToCheckPoint * dirPow;

	//�n���h����؂�ʁB
	float FDotToPoint = carForward.Dot(handleDir);
	float angleHandle = (HANDLE_FROM_ANGLE_END - FDotToPoint) / (HANDLE_FROM_ANGLE_END - HANDLE_FROM_ANGLE_START);
	angleHandle = max(0.0f, angleHandle);
	//�ǂ����Ƀn���h����؂邩�����߂�B
	if (carRight.Dot(handleDir) < 0.0f)
	{
		angleHandle *= -1.0f;
	}

	float HandlePow = (angleHandle + distHandle) / 2.0f;
	float Handle = max(-1.0f, min(1.0f, HandlePow));
	if (fabsf(Handle) < 0.1f) {
		Handle = 0.0f;
	}


	if (fabsf(HandlePow) > 0.8f)
	{
		m_driverInput[enDriverDrift] = true;
		m_driverHandle = Handle;
	}
	else
	{
		m_driverInput[enDriverAccele] = true;
		m_driverHandle = Handle;
	}
}
