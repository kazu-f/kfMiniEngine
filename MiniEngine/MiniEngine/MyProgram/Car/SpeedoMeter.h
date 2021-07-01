#pragma once

class SpeedoMeter
{
public:
	SpeedoMeter();
	~SpeedoMeter();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="pos">表示位置。</param>
	void Init();
	//解放処理。
	void Release();
	//スピードを更新。
	void UpdateSpeed(float speed);

private:
	prefab::CSpriteRender* m_meter = nullptr;			//メーターのスプライト。
	prefab::CSpriteRender* m_needle = nullptr;			//針のスプライト。
	float m_currentSpeed = 0.0f;						//現在の速度。
};

