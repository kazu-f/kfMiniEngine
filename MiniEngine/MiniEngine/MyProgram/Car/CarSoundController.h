#pragma once
class Car;

class CarSoundController
{
public:
	CarSoundController();
	~CarSoundController();

	/// <summary>
	/// サウンドの初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 車のSEを更新する。
	/// </summary>
	void UpdateCarSound(Car* car);
	/// <summary>
	/// 車の速度からエンジンの音の高さを計算。
	/// </summary>
	void CalcSoundFrequencyRetio(Car* car);
	/// <summary>
	/// ブレーキ音の再生。
	/// </summary>
	void PlayBrake(Car* car);
private:
	prefab::CSoundSource* m_engineSE = nullptr;		//エンジン音。
	prefab::CSoundSource* m_brakeSE = nullptr;		//ブレーキ音。
	float m_soundFrequency = 0.0f;					//エンジンの音の周波数？
	bool m_isBraking = false;						//ブレーキ中か。
};

