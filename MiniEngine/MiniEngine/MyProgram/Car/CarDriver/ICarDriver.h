#pragma once
class ICarDriver
{
public:
	enum EnDriverInput {
		enDriverIdle,
		enDriverAccele,
		enDriverBrake,
		enDriverDrift,
		enDriverNum
	};
public:
	virtual ~ICarDriver() {};
	virtual void Update() = 0;
	/// <summary>
	/// 入力を受け取る。
	/// </summary>
	/// <param name="enInput">入力の種類。</param>
	/// <returns>bool型の入力。</returns>
	bool GetDriverInput(EnDriverInput enInput)
	{
		ENGINE_ASSERT(
			0 <= enInput || enInput < enDriverNum,
			"ドライバーの入力の範囲外を指定しています。\n"
			"ICarDriver.hのEnDriverInputを参照してください。"
		);
		return m_driverInput[enInput];
	}
	/// <summary>
	/// ハンドルの入力値を受け取る。
	/// </summary>
	/// <returns>float型の入力。</returns>
	float GetDriverHandle()
	{
		return m_driverHandle;
	}

protected:
	/// <summary>
	/// 入力をリセット。
	/// </summary>
	void ResetDriverInput();

protected:
	bool m_driverInput[enDriverNum] = { false };
	float m_driverHandle = 0.0f;			//ハンドル入力量。(-1.0f ～ 1,0f)

};

