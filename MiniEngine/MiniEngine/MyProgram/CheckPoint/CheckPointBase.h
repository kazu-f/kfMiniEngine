#pragma once

#include "CheckedController.h"

/// <summary>
/// チェックポイント。
/// </summary>
class CheckPointBase
{
public:
	CheckPointBase();
	virtual ~CheckPointBase();
	//初期化処理。
	void Init();
	//キャラコンとの判定。
	virtual void CheckHit(CheckedController* checkedCon) = 0;
	/// <summary>
	/// 逆走判定。
	/// </summary>
	/// <param name="car">チェックされるもの。</param>
	/// <returns>判定結果。trueが逆走している。</returns>
	bool CheckReverseRun(CheckedController* checkedCon);

public:		//セット関係。
	//座標の設定。
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	//回転の設定。
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	//チェックポイント番号を設定。
	void SetCheckPointNum(int num)
	{
		m_checkPointNo = num;
	}
	/// <summary>
	/// 前のチェックポイントを指定。
	/// </summary>
	void SetBackCheckPoint(CheckPointBase* back)
	{
		m_backPoint = back;
	}
	/// <summary>
	/// 次のチェックポイントを指定。
	/// </summary>
	void SetNextCheckPoint(CheckPointBase* next)
	{
		m_nextPoint = next;
	}
public:		//ゲット関係。
	//座標を取得。
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	//チェックポイント番号を取得。
	int GetCheckPointNum()
	{
		return m_checkPointNo;
	}

protected:
	CPhysicsGhostObject m_ghostObj;					//ゴーストオブジェクト。
	Vector3 m_position = Vector3::Zero;				//座標。
	Quaternion m_rotation = Quaternion::Identity;	//回転。
	Vector3 m_forward = Vector3::AxisZ;				//チェックポイントの前方向。
	CheckPointBase* m_backPoint = nullptr;			//後ろのチェックポイント。
	CheckPointBase* m_nextPoint = nullptr;			//次のチェックポイント。
	int m_checkPointNo = -1;						//チェックポイント番号。
};


