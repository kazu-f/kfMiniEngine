#include "stdafx.h"
#include "CheckPointManager.h"
#include "CheckPointNormal.h"

CheckPointManager::CheckPointManager()
{
}

CheckPointManager::~CheckPointManager()
{
}

bool CheckPointManager::Start()
{
	return true;
}

void CheckPointManager::Update()
{
}

void CheckPointManager::OnDestroy()
{
}

void CheckPointManager::MakeCheckPoint(const Vector3& pos, const Quaternion& rot, int checkPointNum)
{
	//チェックポイントを作成する。
	auto checkPoint = std::make_unique<CheckPointNormal>();
	checkPoint->SetPosition(pos);
	checkPoint->SetRotation(rot);
	checkPoint->SetCheckPointNum(checkPointNum);
	checkPoint->Init();
	//リストに積み込む。
	m_checkPointList.push_back(std::move(checkPoint));
}

void CheckPointManager::MakeGoalPoint(const Vector3& pos, const Quaternion& rot)
{
}
