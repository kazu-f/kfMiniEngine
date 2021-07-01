#pragma once
class Car;

class SpeedoMeter
{
public:
	SpeedoMeter(Car* car);
	~SpeedoMeter();
	void UpdateSpeed(float speed);

private:
	Car* m_car = nullptr;		//プレイヤーの車のインスタンス。
	prefab::CSpriteRender* m_meter = nullptr;			//メーターのスプライト。
	prefab::CSpriteRender* m_needle = nullptr;			//針のスプライト。
	float m_currentSpeed = 0.0f;						//現在の速度。
};

