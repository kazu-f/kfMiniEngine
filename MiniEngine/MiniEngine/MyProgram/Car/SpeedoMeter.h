#pragma once

class SpeedoMeter
{
public:
	SpeedoMeter();
	~SpeedoMeter();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="pos">�\���ʒu�B</param>
	void Init();
	//��������B
	void Release();
	//�X�s�[�h���X�V�B
	void UpdateSpeed(float speed);

private:
	prefab::CSpriteRender* m_meter = nullptr;			//���[�^�[�̃X�v���C�g�B
	prefab::CSpriteRender* m_needle = nullptr;			//�j�̃X�v���C�g�B
	float m_currentSpeed = 0.0f;						//���݂̑��x�B
};

