#pragma once
class Car;

class CarSoundController
{
public:
	CarSoundController();
	~CarSoundController();

	/// <summary>
	/// �T�E���h�̏������B
	/// </summary>
	void Init();
	/// <summary>
	/// �T�E���h�̊J�������B
	/// </summary>
	void Release();
	/// <summary>
	/// �Ԃ�SE���X�V����B
	/// </summary>
	void UpdateCarSound(Car* car);
	/// <summary>
	/// �{�����[���ʂ̐ݒ�B
	/// </summary>
	void SetCarVolume(const float volume)
	{
		m_volume = volume;
	}
private:
	/// <summary>
	/// �Ԃ̑��x����G���W���̉��̍������v�Z�B
	/// </summary>
	void CalcSoundFrequencyRetio(Car* car);
	/// <summary>
	/// �u���[�L���̍Đ��B
	/// </summary>
	void PlayBrake(Car* car);
private:
	prefab::CSoundSource* m_engineSE = nullptr;		//�G���W�����B
	prefab::CSoundSource* m_brakeSE = nullptr;		//�u���[�L���B
	float m_soundFrequency = 0.0f;					//�G���W���̉��̎��g���H
	float m_volume = 1.0f;							//�{�����[���ʁB
	bool m_isBraking = false;						//�u���[�L�����B
};

