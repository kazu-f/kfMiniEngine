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
	//メーターのスプライト。
	m_meter = NewGO<prefab::CSpriteRender>(5);
	m_meter->Init(
		METER_FILEPATH,
		256.0f,
		256.0f
		);
	m_meter->SetPosition(METER_POS);
	m_meter->SetPivot(Sprite::DEFAULT_PIVOT);

	//針のスプライト。
	m_needle = NewGO<prefab::CSpriteRender>(7);
	m_needle->Init(
		NEEDLE_FILEPATH,
		12.0f,
		118.0f
		);
	m_needle->SetPosition(METER_POS);
	m_needle->SetPivot({ 0.5f, 0.05f });
	//クオータニオン。
	Quaternion qRot;
	qRot.SetRotationDegZ(MIN_NEEDLE_DEG);
	//針に回転を設定。
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
	//速度から回転の割合を計算。
	float angleWeight = (CarMoveController::MAX_SPEED - speed) / CarMoveController::MAX_SPEED;
	//針の角度を求める。
	float deg = MAX_NEEDLE_DEG * (1.0f - angleWeight) + MIN_NEEDLE_DEG * angleWeight;
	//クオータニオン。
	Quaternion qRot;
	qRot.SetRotationDegZ(deg);
	//針に回転を設定。
	m_needle->SetRotation(qRot);
}
