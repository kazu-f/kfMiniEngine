#include "stdafx.h"
#include "Car.h"
#include "State/ICarState.h"
#include "State/CarStateIdle.h"
#include "State/CarStateAccele.h"
#include "State/CarStateBrake.h"
#include "GameCamera/GameCamera.h"

namespace {
	const float GRAVITY = 980.0f;			//重力加速度。
	const float HANDLE_WEIGHT = 0.3f;		//ハンドルの効き。
	const float CURVE_DEG = 20.0f;			//カーブの角度。
}


Car::Car()
{
	//ステートを初期化。
	m_stateIdle = std::make_unique<CarStateIdle>(this);
	m_stateAccele = std::make_unique<CarStateAccele>(this);
	m_stateBrake = std::make_unique<CarStateBrake>(this);

	//現在のステートを初期化。
	ChangeState(m_stateIdle.get());
}

Car::~Car()
{
}

bool Car::Start()
{
	//3Dモデルを作成。
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/Car/Car.tkm";
	initData.m_fxFilePath = "Assets/shader/model.fx";
	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData);
	//モデルの設定。
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetShadowCasterFlag(true);

	//キャラコンの初期化。
	m_charaCon.Init(
		50.0f,
		50.0f,
		m_position
	);

	return true;
}

void Car::Update()
{
	const float PadX = g_pad[0]->GetLStickXF();
	const float PadY = g_pad[0]->GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();



#if 0
	//回転を軽くかける。
	Quaternion carRot;
	carRot.SetRotationDegY(Math::PI * CURVE_DEG * PadX * DeltaTime);
	m_rotation.Multiply(carRot);

	//車の方向を求める。
	CalcDirection();

	const float MOVE_SPEED = 4000.0f;
	Vector3 camRight = g_camera3D->GetRight();
	camRight.y = 0.0f;
	camRight.Normalize();
	Vector3 camForward = g_camera3D->GetForward();
	camForward.y = 0.0f;
	camForward.Normalize();

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += camRight * PadX * MOVE_SPEED;
	m_moveSpeed += camForward * PadY * MOVE_SPEED;
#else
	//ステート実行。
	m_currentState->Execute();
	//方向を求める。
	Vector3 vMove = m_moveSpeed;
	vMove.y = 0.0f;
	//回転を軽くかける。
	Quaternion dirRot;
	dirRot.SetRotationDegY(Math::PI * CURVE_DEG * PadX * DeltaTime);
	dirRot.Apply(vMove);
	vMove.Normalize();
	//移動方向を求める。
	vMove = m_forward * HANDLE_WEIGHT + vMove * (1.0f - HANDLE_WEIGHT);
	//移動速度を求める。
	m_moveSpeed.x = vMove.x * m_speed;
	m_moveSpeed.z = vMove.z * m_speed;

	Vector3 moveScaler = m_moveSpeed;
	moveScaler.y = 0.0f;
	if (moveScaler.Length() > 1.0f)
	{
		//車への回転を計算。
		vMove.Normalize();
		float angle = m_forward.Dot(vMove);
		angle = min(1.0f, max(0.0f, angle));
		float radian = acosf(angle);
		float RtoV = m_right.Dot(vMove);
		if (RtoV < 0.0f)
		{
			//逆にする。
			radian *= -1.0f;
		}

		m_rotation.AddRotationY(radian);
	}

	//回転から方向を計算。
	CalcDirection();
#endif

	m_moveSpeed.y -= GRAVITY;

	m_position = m_charaCon.Execute(m_moveSpeed, DeltaTime);

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	//if (g_pad[0]->IsTrigger(enButtonX)) {
	//	DeleteGO(this);
	//}
}

void Car::OnDestroy()
{
	DeleteGO(m_model);
}

void Car::ChangeState(ICarState* state)
{
	//変化なし。
	if (m_currentState == state)
	{
		return;
	}

	if (m_currentState != nullptr)
	{
		//現在ステートの事後処理。
		m_currentState->Leave();
	}
	//ステートを切り替え。
	m_currentState = state;
	//事前処理。
	m_currentState->Enter();
}

void Car::CalcDirection()
{
	//クオータニオンから向きを求める。
	Matrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	//向きを代入する。
	m_forward = { mRot.m[2][0],mRot.m[2][1] ,mRot.m[2][2] };
	m_right = { mRot.m[0][0],mRot.m[0][1] ,mRot.m[0][2] };
	m_up = { mRot.m[1][0],mRot.m[1][1] ,mRot.m[1][2] };
}
