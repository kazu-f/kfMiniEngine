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
	/// ���͂��󂯎��B
	/// </summary>
	/// <param name="enInput">���͂̎�ށB</param>
	/// <returns>bool�^�̓��́B</returns>
	const bool GetDriverInput(EnDriverInput enInput) const
	{
		ENGINE_ASSERT(
			0 <= enInput || enInput < enDriverNum,
			"�h���C�o�[�̓��͈͂̔͊O���w�肵�Ă��܂��B\n"
			"ICarDriver.h��EnDriverInput���Q�Ƃ��Ă��������B"
		);
		return m_driverInput[enInput];
	}
	/// <summary>
	/// �n���h���̓��͒l���󂯎��B
	/// </summary>
	/// <returns>float�^�̓��́B</returns>
	const float GetDriverHandle() const
	{
		return m_driverHandle;
	}

protected:
	/// <summary>
	/// ���͂����Z�b�g�B
	/// </summary>
	void ResetDriverInput();

protected:
	bool m_driverInput[enDriverNum] = { false };
	float m_driverHandle = 0.0f;			//�n���h�����͗ʁB(-1.0f �` 1,0f)

};

