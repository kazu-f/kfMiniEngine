#include "stdafx.h"
#include "Guardrail.h"

namespace {
	const float CULL_FAR = 20000.0f;
}

Guardrail::Guardrail()
{
}

Guardrail::~Guardrail()
{
}

bool Guardrail::Start()
{

	if (m_objectsData.size() <= 1)
	{
		ENGINE_MESSAGE_BOX(
			"�K�[�h���[���̃C���X�^���V���O�`��̃f�[�^������Ă��Ȃ��B\n"
			"�C���X�^���V���O�`��ɂ�2�ȏ�K�v�ł��B\n"
		);
		return false;
	}

	//���f���������B
	m_model = NewGO<prefab::ModelRender>(0);
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_modelFilePath;

	//�������B
	m_model->Init(
		modelInitData,
		nullptr,
		0,
		static_cast<int>(m_objectsData.size())
	);
	//�e�̐ݒ�B
	m_model->SetShadowCasterFlag(true);
	m_model->SetShadowReceiverFlag(true);

	m_model->SetCullingFar(CULL_FAR);

	return true;
}

void Guardrail::OnDestroy()
{
	//���f�����폜�B
	DeleteGO(m_model);
}

void Guardrail::Update()
{
	//�C���X�^���V���O�`��p�̃f�[�^���X�V�B
	m_model->BeginUpdateInstancingData();
	for (auto& objData : m_objectsData) {
		m_model->UpdateInstancingData(objData.position, objData.rotation, objData.scale, true);
	}
}

void Guardrail::LoadGuardrail(const char* path, const Vector3& parentPos, const Quaternion& parentRot)
{
	CLevel LoadLevel;

	LoadLevel.Init(path, [&](SLevelObjectData& objData) {
		if (objData.EqualObjectName(L"Guardrail/SM_Guardrail"))
		{
			Vector3 pos = objData.position;
			parentRot.Apply(pos);
			pos += parentPos;
			Quaternion rot;
			rot.Multiply(parentRot, objData.rotation);
			//�C���X�^���V���O�`��p�̃f�[�^���L�^�B
			m_objectsData.push_back({
				pos,
				rot,
				Vector3::One
				});
			//�{�b�N�X�R���C�_�[���쐬�B
			CreateBoxCollider(pos, rot);
		}

		return true;
		});
}

void Guardrail::CreateBoxCollider(const Vector3& pos, const Quaternion& rot)
{
	Vector3 position = pos;
	Vector3 pivot = Vector3::Zero;
	pivot.y += BOX_SIZE.y * (1.0f / 2.0f);
	pivot.z += BOX_SIZE.z * (1.0f / 2.0f);
	rot.Apply(pivot);

	position += pivot;
	ColliderPtr col = std::make_unique<CPhysicsStaticObject>();
	col->CreateBox(position, rot, BOX_SIZE);

	m_colliders.push_back(std::move(col));
}
