#pragma once

#include "CheckPointBase.h"
#include "CheckedController.h"

/// <summary>
/// チェックポイントの管理者。
/// </summary>
class CheckPointManager : public IGameObject
{
public:
	CheckPointManager();
	~CheckPointManager();

	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	/// <summary>
	/// チェックポイントを作成する。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	/// <param name="checkPointNum">チェックポイント番号。</param>
	void MakeCheckPoint(const Vector3& pos, const Quaternion& rot, int checkPointNum);
	/// <summary>
	/// ゴールを作成する。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	void MakeGoalPoint(const Vector3& pos, const Quaternion& rot);

private:
	typedef std::unique_ptr<CheckPointBase> CheckPointPtr;

	std::list<CheckPointPtr> m_checkPointList;				//チェックポイントのリスト。
	std::vector<CheckedController*> m_checkdControllerList;	//チェックされるものリスト。
};

