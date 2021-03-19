#pragma once

#include "CheckPointBase.h"
#include "CheckedController.h"
class CheckPointGoal;

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
	/// <summary>
	/// 初期化を開始する。
	/// </summary>
	void EnableInit()
	{
		m_isStartInit = true;
	}

	/// <summary>
	/// インスタンスを取得する。
	/// </summary>
	CheckPointManager* GetInstance()
	{
		return m_instance;
	}
	/// <summary>
	/// チェックコントローラーを登録する。
	/// </summary>
	void RegistCheckedController(CheckedController* controller)
	{
		m_checkedControllerList.push_back(controller);
	}

private:
	static CheckPointManager* m_instance;

	typedef std::unique_ptr<CheckPointBase> CheckPointPtr;

	std::vector<CheckPointPtr> m_checkPointList;				//チェックポイントのリスト。
	std::vector<CheckedController*> m_checkedControllerList;	//チェックされるものリスト。
	CheckPointGoal* m_goalPoint = nullptr;					//ゴールのアドレス。
	bool m_isStartInit = false;								//初期化開始フラグ。
};

