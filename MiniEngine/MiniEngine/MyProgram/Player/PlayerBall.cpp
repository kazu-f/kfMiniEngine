#include "stdafx.h"
#include "PlayerBall.h"

namespace {
	const char* BALL_MODEL = "Assets/modelData/testbg/Ball.tkm";		//モデルのファイルパス。
	const float SPHERE_SIZE = 30.0f;			//球の半径。
	const float SPHERE_MASS = 3.0f;				//質量。
	const float FRICTION = 0.0f;				//摩擦力。
	const float GRAVITY = 98.0f;			//重力加速度。
	const Vector3 SPHERE_LOCALINTERIA = { 0.0f,0.0f,0.0f };				//慣性テンソル。
	const Vector3 SPHERE_LINARFACTOR = { 1.0f,1.0f,1.0f };				//移動可能軸。
	const Vector3 SPHERE_ANGULARFACTOR = { 1.0f,1.0f,1.0f };			//回転可能軸。
	const Vector3 BOX_SIZE = { 30.0f,30.0f,60.0f };						//ボックスのサイズ。
}


PlayerBall::PlayerBall()
{
}

PlayerBall::~PlayerBall()
{
}

bool PlayerBall::Start()
{
	m_model = NewGO<prefab::ModelRender>(0);
	//モデル情報を設定する。
	ModelInitData modelData;
	modelData.m_tkmFilePath = BALL_MODEL;
	modelData.m_fxFilePath = Model::MODEL_SHADER_PAHT;
	m_model->Init(modelData);

	//コライダーの作成。
	m_sphere.Create(SPHERE_SIZE);

	m_box.Create(BOX_SIZE);

	Vector3 pos = Vector3::Zero;
	pos.y += SPHERE_SIZE;
	//剛体作成。
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_box;
	rbInfo.pos = pos;
	rbInfo.rot = Quaternion::Identity;
	rbInfo.mass = SPHERE_MASS;
	rbInfo.localInteria = SPHERE_LOCALINTERIA;
	m_rigidBody.Create(rbInfo);
	m_rigidBody.SetLinearFactor(SPHERE_LINARFACTOR);
	m_rigidBody.SetAngularFactor(SPHERE_ANGULARFACTOR);
	m_rigidBody.SetFriction(FRICTION);
	m_rigidBody.SetGravity(GRAVITY);

	return true;
}

void PlayerBall::Update()
{
	//剛体の座標と回転を取得。
	Vector3 pos;
	Quaternion rot;
	m_rigidBody.GetPositionAndRotation(pos, rot);
	//剛体の座標と回転をモデルに反映。
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	//剛体に力を加える。
	Vector3 force;
	force.x = -Pad(0).GetLStickXF() * 500.0f;
	force.z = -Pad(0).GetLStickYF() * 500.0f;
	////力を加える
	//m_rigidBody.AddForce(
	//	force,		//力
	//	Vector3::Zero	//力を加える剛体の相対位置
	//);

	m_rigidBody.SetLinerVelocity(force);
}

void PlayerBall::OnDestroy()
{
	DeleteGO(m_model);
}
