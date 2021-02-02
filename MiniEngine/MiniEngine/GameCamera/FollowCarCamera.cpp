#include "stdafx.h"
#include "FollowCarCamera.h"
#include "Car/Car.h"

namespace {
	const Vector3 CAMERA_DISTANCE = { 0.0f,100.0f,-500.0f };		//カメラの最初のターゲットからの位置。
	const float CAMERA_FAR = 10000.0f;							//カメラの遠平面。
	const float TARGET_HIGHT = 80.0f;							//カメラのターゲットの座標からの高さ。
	const float FOLLOW_WEIGHT = 0.7f;							//追従する強さ(0.0～1.0)。1.0に近いほど追従が強い。
}

FollowCarCamera::FollowCarCamera()
{
}

FollowCarCamera::~FollowCarCamera()
{
}

bool FollowCarCamera::Start()
{
	//nullの場合は始まらない。
	if (m_car == nullptr)
	{
		ENGINE_LOG(
			"FollowCarCamera::Start():"
			"車のアドレスがnullでした。"
			"SetTargetCar関数で指定してください。"
		);
		return false;
	}

	//遠平面の設定。
	g_camera3D->SetFar(CAMERA_FAR);

	m_target = m_car->GetPosition();	//ターゲットとなる車の座標を取得。
	m_target.y += TARGET_HIGHT;			//少し高くする。
	m_position = m_target + CAMERA_DISTANCE;

	g_camera3D->SetPosition(m_position);
	g_camera3D->SetTarget(m_target);


	return true;
}

void FollowCarCamera::Update()
{
	//車のアドレスがない。
	if (m_car == nullptr)
	{
		return;
	}
	//車が消えたらnullを入れる。
	if (m_car->IsDead())
	{
		m_car = nullptr;
		return;
	}
	//XZ平面上のベクトルを求める。
	Vector3 toCameraPosXZ = m_position - m_target;
	//ベクトルのY成分は取り出しておく。
	float toCameraPosY = toCameraPosXZ.y;
	toCameraPosXZ.y = 0.0f;
	//ベクトルの長さを記録しておく。
	float toCameraPosXZLen = toCameraPosXZ.Length();
	toCameraPosXZ.Normalize();		//正規化。

	//ターゲットの新しい座標を取得。
	Vector3 targetPos = m_car->GetPosition();
	targetPos.y += TARGET_HIGHT;

	//新しい注視点と現在のカメラの視点を使ってXZ平面でのベクトルを求める。
	Vector3 toNewCameraPos = m_position - targetPos;
	toNewCameraPos.y = 0.0f;
	toNewCameraPos.Normalize();

	//視点の位置を計算していく。
	toNewCameraPos = toNewCameraPos * FOLLOW_WEIGHT + toCameraPosXZ * (1.0f - FOLLOW_WEIGHT);
	toNewCameraPos.Normalize();			//正規化。
	//記録しておいたベクトルの長さを掛けて視点までのベクトルを出す。
	toNewCameraPos *= toCameraPosXZLen;
	//高さを戻す。
	toNewCameraPos.y = toCameraPosY;

	//座標をカメラに設定する。
	m_position = targetPos + toNewCameraPos;
	m_target = targetPos;

	//カメラの座標を更新。
	g_camera3D->SetTarget(m_target);
	g_camera3D->SetPosition(m_position);
}

void FollowCarCamera::OnDestroy()
{
}
