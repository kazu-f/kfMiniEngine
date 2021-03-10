#pragma once

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
	void Init(CCharacterController* chara, Vector3& pos, Quaternion& rot);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	void Update(Vector3& pos, Quaternion& rot);
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
		m_currentCheckPointNum = checkPointNum;
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
	/// キャラコンを取得する。
	/// </summary>
	CCharacterController* GetCharaCon()
	{
		return m_characon;
	}
	bool IsReverseRun()const
	{
		return m_isReverseRun;
	}

private:
	/// <summary>
	/// 回転から前方向を計算。
	/// </summary>
	void CalcForward();

private:
	CCharacterController* m_characon = nullptr;			//キャラコン。
	Vector3 m_position = Vector3::Zero;					//座標。
	Quaternion m_rotation = Quaternion::Identity;		//回転。
	Vector3 m_forward = Vector3::AxisZ;					//前方向。
	int m_currentCheckPointNum = 0;						//現在の次のチェックポイント番号。
	int m_currentLapsNum = 0;							//現在の周回数。
	bool m_isReverseRun = false;						//逆走中か？
};

