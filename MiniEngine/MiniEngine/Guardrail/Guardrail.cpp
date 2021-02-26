#include "stdafx.h"
#include "Guardrail.h"

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
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	modelInitData.m_vsEntryPointFunc = "VSMainInstancing";

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

	return true;
}

void Guardrail::OnDestroy()
{
}

void Guardrail::Update()
{
	//�C���X�^���V���O�`��p�̃f�[�^���X�V�B
	if (m_numRenderObjects < m_objectsData.size()) {
		for (auto& objData : m_objectsData) {
			m_model->UpdateInstancingData(objData.position, objData.rotation, objData.scale);
			m_numRenderObjects++;
		}
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
	Vector3 pivot = pos;
	pivot += BOX_SIZE * (1.0f / 2.0f);
	ColliderPtr col = std::make_unique<CPhysicsStaticObject>();
	col->CreateBox(pivot, rot, BOX_SIZE);

	m_colliders.push_back(std::move(col));
}