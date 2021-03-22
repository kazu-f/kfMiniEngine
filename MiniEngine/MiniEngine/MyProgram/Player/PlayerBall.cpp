#include "stdafx.h"
#include "PlayerBall.h"

namespace {
	const char* BALL_MODEL = "Assets/modelData/testbg/Ball.tkm";		//���f���̃t�@�C���p�X�B
	const float SPHERE_SIZE = 30.0f;			//���̔��a�B
	const float SPHERE_MASS = 3.0f;				//���ʁB
	const float FRICTION = 0.0f;				//���C�́B
	const float GRAVITY = 98.0f;			//�d�͉����x�B
	const Vector3 SPHERE_LOCALINTERIA = { 0.0f,0.0f,0.0f };				//�����e���\���B
	const Vector3 SPHERE_LINARFACTOR = { 1.0f,1.0f,1.0f };				//�ړ��\���B
	const Vector3 SPHERE_ANGULARFACTOR = { 1.0f,1.0f,1.0f };			//��]�\���B
	const Vector3 BOX_SIZE = { 30.0f,30.0f,60.0f };						//�{�b�N�X�̃T�C�Y�B
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
	//���f������ݒ肷��B
	ModelInitData modelData;
	modelData.m_tkmFilePath = BALL_MODEL;
	modelData.m_fxFilePath = Model::MODEL_SHADER_PAHT;
	m_model->Init(modelData);

	//�R���C�_�[�̍쐬�B
	m_sphere.Create(SPHERE_SIZE);

	m_box.Create(BOX_SIZE);

	Vector3 pos = Vector3::Zero;
	pos.y += SPHERE_SIZE;
	//���̍쐬�B
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
	//���̂̍��W�Ɖ�]���擾�B
	Vector3 pos;
	Quaternion rot;
	m_rigidBody.GetPositionAndRotation(pos, rot);
	//���̂̍��W�Ɖ�]�����f���ɔ��f�B
	m_model->SetPosition(pos);
	m_model->SetRotation(rot);
	//���̂ɗ͂�������B
	Vector3 force;
	force.x = -Pad(0).GetLStickXF() * 500.0f;
	force.z = -Pad(0).GetLStickYF() * 500.0f;
	////�͂�������
	//m_rigidBody.AddForce(
	//	force,		//��
	//	Vector3::Zero	//�͂������鍄�̂̑��Έʒu
	//);

	m_rigidBody.SetLinerVelocity(force);
}

void PlayerBall::OnDestroy()
{
	DeleteGO(m_model);
}
