#include "stdafx.h"
#include "Car.h"
#include "State/ICarState.h"
#include "State/CarStateIdle.h"
#include "State/CarStateAccele.h"
#include "State/CarStateBrake.h"
#include "GameCamera/GameCamera.h"
#include "CheckPoint/CheckedController.h"

#define ISRigidBody 0

namespace {
	const float GRAVITY = 980.0f;			//重力加速度。
	const float HANDLE_WEIGHT = 0.3f;		//ハンドルの効き。
	const float CURVE_DEG = 20.0f;			//カーブの角度。
	const float DRIFT_POWER = 1.4f;			//ドリフトの強さ。

	const float DEADROT_SPEED = 0.1f;		//速度が一定以下なら回転しない。

	const Vector3 CAR_SIZE = { 150.0f,150.0f,300.0f };			//車のサイズ。
	const float HEIGHT_DIF = 300.0f;							//コライダーとの差分。
	const Vector3 CAR_LOCALINTERIA = { 0.0f,0.0f,0.0f };		//車の回転のしやすさ。
	const Vector3 CAR_LINERFACTOR = { 1.0f,1.0f,1.0f };			//移動可能軸。
	const Vector3 CAR_ANGULARFACTOR = { 1.0f,1.0f,1.0f };		//回転軸を設定。
	const float CAR_MASS = 3.0f;								//車の質量。
	const float CAR_FRICTION = 0.0f;							//車の摩擦力。

}

const float Car::MOVE_COEFFICIENT = 60.0f;

Car::Car()
{
	//ステートを初期化。
	m_stateIdle = std::make_unique<CarStateIdle>(this);
	m_stateAccele = std::make_unique<CarStateAccele>(this);
	m_stateBrake = std::make_unique<CarStateBrake>(this);
	m_checkedCon = std::make_unique<CheckedController>();
}

Car::~Car()
{
}

bool Car::Start()
{
	//3Dモデルを作成。
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/Car/Car.tkm";
	m_model = NewGO<prefab::ModelRender>(0);
	m_model->Init(initData);
	//モデルの設定。
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetShadowCasterFlag(true);
	m_model->SetShadowReceiverFlag(true);

#if ISRigidBody
	//車のコリジョンを作成。
	m_carCollider.Create(CAR_SIZE);

	Vector3 pos = m_position;
	pos.y += HEIGHT_DIF;
	//剛体を作成する。
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_carCollider;
	rbInfo.pos = pos;
	rbInfo.rot = m_rotation;
	rbInfo.localInteria = CAR_LOCALINTERIA;
	rbInfo.mass = CAR_MASS;
	m_rigidBody.Create(rbInfo);
	//摩擦力の設定。
	m_rigidBody.SetFriction(CAR_FRICTION);
	//移動可能軸。
	m_rigidBody.SetLinearFactor(CAR_LINERFACTOR);
	//回転軸を設定。
	m_rigidBody.SetAngularFactor(CAR_ANGULARFACTOR);
	//重力を設定。
	m_rigidBody.SetGravity(GRAVITY * 100.0f);

	//チェックポイントコントローラーの初期化。
	m_checkedCon->Init(
		&m_rigidBody,
		m_position,
		m_rotation
	);
#else

	//キャラコンの初期化。
	m_charaCon.Init(
		50.0f,
		50.0f,
		m_position
	);

	//チェックポイントコントローラーの初期化。
	m_checkedCon->Init(
		m_charaCon.GetBody(),
		m_position,
		m_rotation
	);
#endif

	//現在のステートを初期化。
	ChangeState(m_stateIdle.get());

	return true;
}

void Car::Update()
{
	if (m_currentState == nullptr) return;

#if ISRigidBody
	RigidBodyMove();
#else
	CharaConMove();
#endif

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	m_checkedCon->Update(m_position, m_rotation);

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

void Car::CharaConMove()
{
	const float PadX = Pad(0).GetLStickXF();
	const float PadY = Pad(0).GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	//ステート実行。
	m_currentState->Execute();
	//方向を求める。
	Vector3 vMove = m_moveSpeed;
	vMove.y = 0.0f;
	//回転を軽くかける。
	Quaternion dirRot;
	float rotSpeed = CURVE_DEG;
	if (m_currentState->IsDrift()) {
		//ブレーキ中はカーブしやすい。
		rotSpeed *= DRIFT_POWER;
	}
	dirRot.SetRotationDegY(Math::PI * rotSpeed * PadX * DeltaTime);
	dirRot.Apply(vMove);
	vMove.Normalize();
	//移動方向を求める。
	vMove = m_forward * HANDLE_WEIGHT + vMove * (1.0f - HANDLE_WEIGHT);
	//移動速度を求める。
	m_moveSpeed.x = vMove.x * m_speed;
	m_moveSpeed.z = vMove.z * m_speed;

	Vector3 moveScaler = m_moveSpeed;
	moveScaler.y = 0.0f;
	//動きがある。
	if (moveScaler.Length() > DEADROT_SPEED)
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
		if (fabs(radian) > 0.01f) {
			m_rotation.AddRotationY(radian);
		}
	}

	//回転から方向を計算。
	CalcDirection();

	m_moveSpeed.y -= GRAVITY;

	m_position = m_charaCon.Execute(m_moveSpeed, DeltaTime);
}

void Car::RigidBodyMove()
{
	const float PadX = Pad(0).GetLStickXF();
	const float PadY = Pad(0).GetLStickYF();
	const float DeltaTime = GameTime().GetFrameDeltaTime();

	m_rigidBody.GetPositionAndRotation(m_position, m_rotation);
	m_position.y -= CAR_SIZE.y / 2.0f;

	//ステート実行。
	m_currentState->Execute();
	//方向を求める。
	Vector3 vMove = m_moveSpeed;
	vMove.y = 0.0f;
	//回転を軽くかける。
	Quaternion dirRot;
	float rotSpeed = CURVE_DEG;
	if (m_currentState->IsDrift()) {
		//ブレーキ中はカーブしやすい。
		rotSpeed *= DRIFT_POWER;
	}
	dirRot.SetRotationDegY(Math::PI * rotSpeed * PadX * DeltaTime);
	dirRot.Apply(vMove);
	vMove.Normalize();
	//移動方向を求める。
	vMove = m_forward * HANDLE_WEIGHT + vMove * (1.0f - HANDLE_WEIGHT);
	//移動速度を求める。
	m_moveSpeed.x = vMove.x * m_speed;
	m_moveSpeed.z = vMove.z * m_speed;

	//回転を求める。
	float radian = 0.0f;
	Vector3 moveScaler = m_moveSpeed;
	moveScaler.y = 0.0f;
	//動きがある。
	if (moveScaler.Length() > DEADROT_SPEED)
	{
		//車への回転を計算。
		vMove.Normalize();
		float angle = m_forward.Dot(vMove);
		angle = min(1.0f, max(0.0f, angle));
		radian = acosf(angle);
		float RtoV = m_right.Dot(vMove);
		if (RtoV < 0.0f)
		{
			//逆にする。
			radian *= -1.0f;
		}
		if (fabs(radian) > 0.01f) {
			m_rotation.AddRotationY(radian);
		}
	}

	//回転から方向を計算。
	CalcDirection();

	//m_moveSpeed.y -= GRAVITY;

	m_rigidBody.SetLinerVelocity(m_moveSpeed);
	//m_rigidBody.AddForce(m_moveSpeed * DeltaTime,Vector3::Zero);
	m_rigidBody.SetAngularVelocity({ 0.0f, radian * CURVE_DEG, 0.0f });
}
