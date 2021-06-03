#include "stdafx.h"
#include "AICarDriver.h"
#include "CheckPoint/CheckPointManager.h"
#include "Car/Car.h"

namespace {
	//ハンドル量を線形変換するための変数。
	const float HANDLE_FROM_ANGLE_START = 0.8f;
	const float HANDLE_FROM_ANGLE_END = 1.0f;
	const float HANDLE_FROM_DIST_START = 800.0f;
	const float HANDLE_FROM_DIST_END = 300.0f;
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

	//チェックポイントの座標を取得。
	int currentNo = checkCon->GetCheckPointNo();
	CheckPointBase* currentCheckPoint = checkMgr->GetCheckPoint(currentNo);
	Vector3 currentCheckPointPos = currentCheckPoint->GetPosition();
	Vector3 backCheckPointPos = currentCheckPoint->GetBackCheckPoint()->GetPosition();
	
	//次のチェックポイントへの角度からハンドル量を求める。
	//車からチェックポイントへのベクトル。
	Vector3 CarToCheckPoint = currentCheckPointPos - pos;
	CarToCheckPoint.y = 0.0f;		//XZ平面化。
	CarToCheckPoint.Normalize();

	//車の方向。
	Vector3 forward = checkCon->GetForward();
	Vector3 right = checkCon->GetRight();
	//ハンドルを切る量。
	float FDotToPoint = forward.Dot(CarToCheckPoint);
	float angleHandle = (HANDLE_FROM_ANGLE_END - FDotToPoint) / (HANDLE_FROM_ANGLE_END - HANDLE_FROM_ANGLE_START);
	angleHandle = max(0.0f, angleHandle);
	if (FDotToPoint > 0.9f)
	{
		angleHandle = 0.0f;
	}

	//チェックポイントを繋ぐ線分からの距離でハンドル量を求める。
	//チェックポイントを繋ぐ方向ベクトルを求める。
	Vector3 checkPointDir = currentCheckPointPos - backCheckPointPos;
	checkPointDir.y = 0.0f;
	checkPointDir.Normalize();
	//手前のチェックポイントから車までのベクトル。
	Vector3 backCheckPointToCar = pos - backCheckPointPos;
	backCheckPointToCar.y = 0.0f;

	//線分からの距離を求めていく。
	float backToCarDist = checkPointDir.Dot(backCheckPointToCar);
	Vector3 onLineCarPos = backCheckPointPos + (checkPointDir * backToCarDist);		//線分上の位置。
	Vector3 lineToCar = pos - onLineCarPos;
	float len = lineToCar.Length();
	float distHandle = (HANDLE_FROM_DIST_END - len) / (HANDLE_FROM_DIST_END - HANDLE_FROM_DIST_START);
	distHandle = max(0.0f, distHandle);

	float Handle = (angleHandle + distHandle) / 2.0f;
	Handle = max(0.0f, min(1.0f, Handle));
	//どっちにハンドルを切るかを決める。
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
