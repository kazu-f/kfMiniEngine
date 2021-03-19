#include "stdafx.h"
#include "CheckPointManager.h"
#include "CheckPointNormal.h"
#include "CheckPointGoal.h"

CheckPointManager* CheckPointManager::m_instance = nullptr;

CheckPointManager::CheckPointManager()
{
	//既にあるため不要。
	if (m_instance != nullptr)
	{
		ENGINE_WARNING_LOG(
			"CheckPointManager::CheckPointManager()",
			" 既にインスタンスが存在している。"
		);
		DeleteGO(this);
	}
	else
	{
		m_instance = this;
	}
}

CheckPointManager::~CheckPointManager()
{
	if (m_instance == this)
	{
		m_instance = nullptr;
	}
}

bool CheckPointManager::Start()
{
	//初期化を開始しない。
	if (!m_isStartInit)return false;
	//リストのサイズを取得。
	int listSize = static_cast<int>(m_checkPointList.size());
	m_goalPoint->SetCheckPointNum(listSize - 1);		//ゴールのチェックポイント番号は最後。
	//チェックポイント番号にソート。
	std::sort(
		m_checkPointList.begin(),
		m_checkPointList.end(),
		[&](CheckPointPtr& a, CheckPointPtr& b) {
			return a->GetCheckPointNum() < b->GetCheckPointNum();
		}
	);
	//それぞれのチェックポイントに後ろと次のチェックポイントを記録する。
	for (int i = 0; i < listSize; i++)
	{
		//手前のチェックポイントを記憶させる。
		m_checkPointList[i]->SetBackCheckPoint(m_checkPointList[(i + listSize - 1) % listSize].get());
		//次のチェックポイントを記憶させる。
		m_checkPointList[i]->SetNextCheckPoint(m_checkPointList[(i + 1) % listSize].get());
	}


	return true;
}

void CheckPointManager::Update()
{
	int checkPointSize = static_cast<int>(m_checkPointList.size());
	for (auto* controller : m_checkedControllerList) {
		int indexNo = controller->GetCheckPointNo();
		//逆走中か？
		controller->SetIsReverseRun(m_checkPointList[indexNo]->CheckReverseRun(controller));
		//チェックポイントの更新があるか？
		m_checkPointList[(indexNo + checkPointSize - 1) % checkPointSize]->CheckCharaconHit(controller);
		m_checkPointList[indexNo]->CheckCharaconHit(controller);
		m_checkPointList[(indexNo + 1) % checkPointSize]->CheckCharaconHit(controller);
	}
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
	//既にある。
	if (m_goalPoint != nullptr)
	{
		ENGINE_LOG(
			"既にゴールは存在しています。",
			" : レベルファイルを確認。")
		return;
	}
	//ゴールのチェックポイントを作る。
	auto goalPoint = std::make_unique<CheckPointGoal>();
	goalPoint->SetPosition(pos);
	goalPoint->SetRotation(rot);
	goalPoint->Init();
	//ゴールだけポインタを取得しておく。
	m_goalPoint = goalPoint.get();
	//リストに積み込む。
	m_checkPointList.push_back(std::move(goalPoint));
}
