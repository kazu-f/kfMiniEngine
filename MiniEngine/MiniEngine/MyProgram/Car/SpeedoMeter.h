#pragma once
class Car;

class SpeedoMeter
{
public:
	SpeedoMeter(Car* car);
	~SpeedoMeter();
	void UpdateSpeed(float speed);

private:
	Car* m_car = nullptr;		//�v���C���[�̎Ԃ̃C���X�^���X�B
	prefab::CSpriteRender* m_meter = nullptr;			//���[�^�[�̃X�v���C�g�B
	prefab::CSpriteRender* m_needle = nullptr;			//�j�̃X�v���C�g�B
	float m_currentSpeed = 0.0f;						//���݂̑��x�B
};

