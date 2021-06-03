#include "stdafx.h"
#include "AICarDriver.h"
#include "CheckPoint/CheckPointManager.h"
#include "Car/Car.h"

namespace {
	//�n���h���ʂ���`�ϊ����邽�߂̕ϐ��B
	const float HANDLE_FROM_ANGLE_START = 0.8f;
	const float HANDLE_FROM_ANGLE_END = 1.0f;
	const float HANDLE_FROM_DIST_START = 800.0f;
	const float HANDLE_FROM_DIST_END = 300.0f;
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

	//�`�F�b�N�|�C���g�̍��W���擾�B
	int currentNo = checkCon->GetCheckPointNo();
	CheckPointBase* currentCheckPoint = checkMgr->GetCheckPoint(currentNo);
	Vector3 currentCheckPointPos = currentCheckPoint->GetPosition();
	Vector3 backCheckPointPos = currentCheckPoint->GetBackCheckPoint()->GetPosition();
	
	//���̃`�F�b�N�|�C���g�ւ̊p�x����n���h���ʂ����߂�B
	//�Ԃ���`�F�b�N�|�C���g�ւ̃x�N�g���B
	Vector3 CarToCheckPoint = currentCheckPointPos - pos;
	CarToCheckPoint.y = 0.0f;		//XZ���ʉ��B
	CarToCheckPoint.Normalize();

	//�Ԃ̕����B
	Vector3 forward = checkCon->GetForward();
	Vector3 right = checkCon->GetRight();
	//�n���h����؂�ʁB
	float FDotToPoint = forward.Dot(CarToCheckPoint);
	float angleHandle = (HANDLE_FROM_ANGLE_END - FDotToPoint) / (HANDLE_FROM_ANGLE_END - HANDLE_FROM_ANGLE_START);
	angleHandle = max(0.0f, angleHandle);
	if (FDotToPoint > 0.9f)
	{
		angleHandle = 0.0f;
	}

	//�`�F�b�N�|�C���g���q����������̋����Ńn���h���ʂ����߂�B
	//�`�F�b�N�|�C���g���q�������x�N�g�������߂�B
	Vector3 checkPointDir = currentCheckPointPos - backCheckPointPos;
	checkPointDir.y = 0.0f;
	checkPointDir.Normalize();
	//��O�̃`�F�b�N�|�C���g����Ԃ܂ł̃x�N�g���B
	Vector3 backCheckPointToCar = pos - backCheckPointPos;
	backCheckPointToCar.y = 0.0f;

	//��������̋��������߂Ă����B
	float backToCarDist = checkPointDir.Dot(backCheckPointToCar);
	Vector3 onLineCarPos = backCheckPointPos + (checkPointDir * backToCarDist);		//������̈ʒu�B
	Vector3 lineToCar = pos - onLineCarPos;
	float len = lineToCar.Length();
	float distHandle = (HANDLE_FROM_DIST_END - len) / (HANDLE_FROM_DIST_END - HANDLE_FROM_DIST_START);
	distHandle = max(0.0f, distHandle);

	float Handle = (angleHandle + distHandle) / 2.0f;
	Handle = max(0.0f, min(1.0f, Handle));
	//�ǂ����Ƀn���h����؂邩�����߂�B
	float dir = right.Dot(CarToCheckPoint);
	if (dir < 0.0f)
	{
		Handle *= -1.0f;
	}

	if (FDotToPoint < 0.4f)
	{
		m_driverInput[enDriverBrake] = true;
		m_driverHandle = Handle;
	}
	else if (FDotToPoint < 0.9f || distHandle > 1.0f)
	{
		m_driverInput[enDriverAccele] = true;
		m_driverHandle = Handle;
	}
	else {
		m_driverInput[enDriverAccele] = true;
		m_driverHandle = 0.0f;
	}
}
