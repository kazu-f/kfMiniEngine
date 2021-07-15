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
	/// �Ԃ�SE���X�V����B
	/// </summary>
	void UpdateCarSound(Car* car);
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
	bool m_isBraking = false;						//�u���[�L�����B
};

