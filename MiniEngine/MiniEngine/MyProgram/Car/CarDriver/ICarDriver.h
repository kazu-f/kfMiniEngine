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
	bool GetDriverInput(EnDriverInput enInput)
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
	float GetDriverHandle()
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

