#include "stdafx.h"
#include "SpeedoMeter.h"
#include "CarMoveController.h"

namespace {
	const char* NEEDLE_FILEPATH = "Assets/sprite/Car/Needle.dds";
	const char* METER_FILEPATH = "Assets/sprite/Car/SpeedoMeter.dds";

	const Vector3 METER_POS = { -500.0f,-260.0f,0.0f };

	const float MAX_NEEDLE_DEG = -114.0f;
	const float MIN_NEEDLE_DEG = 114.0f;
}

SpeedoMeter::SpeedoMeter()
{
}

SpeedoMeter::~SpeedoMeter()
{
	Release();
}

void SpeedoMeter::Init()
{
	//���[�^�[�̃X�v���C�g�B
	m_meter = NewGO<prefab::CSpriteRender>(5);
	m_meter->Init(
		METER_FILEPATH,
		256.0f,
		256.0f
		);
	m_meter->SetPosition(METER_POS);
	m_meter->SetPivot(Sprite::DEFAULT_PIVOT);

	//�j�̃X�v���C�g�B
	m_needle = NewGO<prefab::CSpriteRender>(7);
	m_needle->Init(
		NEEDLE_FILEPATH,
		12.0f,
		118.0f
		);
	m_needle->SetPosition(METER_POS);
	m_needle->SetPivot({ 0.5f, 0.05f });
	//�N�I�[�^�j�I���B
	Quaternion qRot;
	qRot.SetRotationDegZ(MIN_NEEDLE_DEG);
	//�j�ɉ�]��ݒ�B
	m_needle->SetRotation(qRot);

}

void SpeedoMeter::Release()
{
	if (m_needle != nullptr)
	{
		DeleteGO(m_needle);
		m_needle = nullptr;
	}
	if (m_meter != nullptr)
	{
		DeleteGO(m_meter);
		m_meter = nullptr;
	}
}

void SpeedoMeter::UpdateSpeed(float speed)
{
	//���x�����]�̊������v�Z�B
	float angleWeight = (CarMoveController::MAX_SPEED - speed) / CarMoveController::MAX_SPEED;
	//�j�̊p�x�����߂�B
	float deg = MAX_NEEDLE_DEG * (1.0f - angleWeight) + MIN_NEEDLE_DEG * angleWeight;
	//�N�I�[�^�j�I���B
	Quaternion qRot;
	qRot.SetRotationDegZ(deg);
	//�j�ɉ�]��ݒ�B
	m_needle->SetRotation(qRot);
}
