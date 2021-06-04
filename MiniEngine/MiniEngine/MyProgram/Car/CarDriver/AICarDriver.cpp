#include "stdafx.h"
#include "AICarDriver.h"
#include "CheckPoint/CheckPointManager.h"
#include "Car/Car.h"

namespace {
	//ハンドル量を線形変換するための変数。
	const float HANDLE_FROM_ANGLE_START = 0.8f;
	const float HANDLE_FROM_ANGLE_END = 1.0f;
	const float HANDLE_FROM_DIST_START = 600.0f;
	const float HANDLE_FROM_DIST_END = 100.0f;
}

void AICarDriver::Update()
{
	//ドライバーの入力をリセット。
	ResetDriverInput();

	CheckPointManager* checkMgr = CheckPointManager::GetInstance();
	if (checkMgr == nullptr) return;

	//現在の座標を取得。
	auto checkCon = m_car->GetCheckedController();
	Vector3 pos = checkCon->GetPosition();
	pos.y = 0.0f;

	//チェックポイントの座標を取得。
	int currentNo = checkCon->GetCheckPointNo();
	CheckPointBase* currentCheckPoint = checkMgr->GetCheckPoint(currentNo);
	Vector3 currentCheckPointPos = currentCheckPoint->GetPosition();
	Vector3 backCheckPointPos = currentCheckPoint->GetBackCheckPoint()->GetPosition();
	
	//チェックポイントを繋ぐ線分からの距離でハンドル量を求める。
	//チェックポイントを繋ぐ方向ベクトルを求める。
	Vector3 pointToPointDir = currentCheckPointPos - backCheckPointPos;
	pointToPointDir.y = 0.0f;
	float checkPointLen = pointToPointDir.Length();
	pointToPointDir.Normalize();
	//手前のチェックポイントから車までのベクトル。
	Vector3 backCheckPointToCar = pos - backCheckPointPos;
	backCheckPointToCar.y = 0.0f;

	//線分からの距離を求めていく。
	float backToCarDist = pointToPointDir.Dot(backCheckPointToCar);
	Vector3 onLineCarPos = backCheckPointPos + (pointToPointDir * backToCarDist);		//線分上の位置。
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


	//車の方向。
	Vector3 carForward = checkCon->GetForward();
	Vector3 carRight = checkCon->GetRight();
	//次のチェックポイントへの角度からハンドル量を求める。
	//車からチェックポイントへのベクトル。
	Vector3 CarToCheckPoint = currentCheckPointPos - pos;
	CarToCheckPoint.y = 0.0f;		//XZ平面化。
	CarToCheckPoint.Normalize();

	//ハンドルを切るべき向きを調べる。
	float dirPow = backToCarDist / checkPointLen;
	dirPow = max(0.0f, min(1.0f, dirPow));
	//Vector3 handleDir = pointToPointDir * (1.0f - dirPow) + currentCheckPoint->GetDirection() * dirPow;
	Vector3 handleDir = currentCheckPoint->GetBackCheckPoint()->GetDirection() * (1.0f - dirPow) + currentCheckPoint->GetDirection() * dirPow;
	//Vector3 handleDir = currentCheckPoint->GetBackCheckPoint()->GetDirection() * (1.0f - dirPow) + CarToCheckPoint * dirPow;

	//ハンドルを切る量。
	float FDotToPoint = carForward.Dot(handleDir);
	float angleHandle = (HANDLE_FROM_ANGLE_END - FDotToPoint) / (HANDLE_FROM_ANGLE_END - HANDLE_FROM_ANGLE_START);
	angleHandle = max(0.0f, angleHandle);
	//どっちにハンドルを切るかを決める。
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
