#pragma once
#include "Race/RaceEnum.h"

/// <summary>
/// チェックポイントから判定されるもの。
/// </summary>
class CheckedController
{
public:
	CheckedController();
	~CheckedController();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="chara">キャラコン。</param>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	void Init(CRigidBody* rb, const Vector3& pos, const Quaternion& rot);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	void Update(const Vector3& pos, const Quaternion& rot);
	/// <summary>
	/// 周回数をカウントアップする。
	/// </summary>
	void CountUpLap()
	{
		m_currentLapsNum++;
	}
	/// <summary>
	/// 周回数を下げる。
	/// </summary>
	void CountDownLap()
	{
		m_currentLapsNum--;
	}
	/// <summary>
	/// チェックポイント番号を設定する。
	/// </summary>
	/// <param name="checkPointNum"></param>
	void SetCheckPointNum(int checkPointNum)
	{
		m_currentCheckPointNo = checkPointNum;
	}
	/// <summary>
	/// 逆走中かどうかのフラグを設定。
	/// </summary>
	/// <param name="flag">trueが逆走中。</param>
	void SetIsReverseRun(bool flag)
	{
		m_isReverseRun = flag;
	}
public:		//ゲット関係。
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 前方向を取得する。
	/// </summary>
	const Vector3& GetForward()const
	{
		return m_forward;
	}
	/// <summary>
	/// 右方向を取得する。
	/// </summary>
	const Vector3& GetRight()const
	{
		return m_right;
	}
	/// <summary>
	/// 現在のチェックポイント番号を取得する。
	/// </summary>
	int GetCheckPointNo()
	{
		return m_currentCheckPointNo;
	}
	/// <summary>
	/// 現在の周回数を取得する。
	/// </summary>
	int GetCurrentLapNum()
	{
		return m_currentLapsNum;
	}
	/// <summary>
	/// キャラコンを取得する。
	/// </summary>
	CRigidBody* GetBody()
	{
		return m_rigidBody;
	}
	/// <summary>
	/// 逆走中か？
	/// </summary>
	/// <returns>逆走中ならtrue。</returns>
	bool IsReverseRun()const
	{
		return m_isReverseRun;
	}

	bool IsGoal()const
	{
		return m_currentLapsNum >= Race::enLap_Num;
	}

private:
	/// <summary>
	/// 回転から方向を計算。
	/// </summary>
	void CalcDirection();

private:
	CRigidBody* m_rigidBody = nullptr;					//判定を行う剛体。
	Vector3 m_position = Vector3::Zero;					//座標。
	Quaternion m_rotation = Quaternion::Identity;		//回転。
	Vector3 m_forward = Vector3::AxisZ;					//前方向。
	Vector3 m_right = Vector3::AxisX;					//右方向。
	int m_currentCheckPointNo = 0;						//現在の次のチェックポイント番号。
	int m_currentLapsNum = 0;							//現在の周回数。
	bool m_isReverseRun = false;						//逆走中か？
};

