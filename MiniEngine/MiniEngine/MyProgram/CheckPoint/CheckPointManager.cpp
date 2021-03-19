#include "stdafx.h"
#include "CheckPointManager.h"
#include "CheckPointNormal.h"
#include "CheckPointGoal.h"

CheckPointManager* CheckPointManager::m_instance = nullptr;

CheckPointManager::CheckPointManager()
{
	//���ɂ��邽�ߕs�v�B
	if (m_instance != nullptr)
	{
		ENGINE_WARNING_LOG(
			"CheckPointManager::CheckPointManager()",
			" ���ɃC���X�^���X�����݂��Ă���B"
		);
		DeleteGO(this);
	}
	else
	{
		m_instance = this;
	}
}

CheckPointManager::~CheckPointManager()
{
	if (m_instance == this)
	{
		m_instance = nullptr;
	}
}

bool CheckPointManager::Start()
{
	//���������J�n���Ȃ��B
	if (!m_isStartInit)return false;
	//���X�g�̃T�C�Y���擾�B
	int listSize = static_cast<int>(m_checkPointList.size());
	m_goalPoint->SetCheckPointNum(listSize - 1);		//�S�[���̃`�F�b�N�|�C���g�ԍ��͍Ō�B
	//�`�F�b�N�|�C���g�ԍ��Ƀ\�[�g�B
	std::sort(
		m_checkPointList.begin(),
		m_checkPointList.end(),
		[&](CheckPointPtr& a, CheckPointPtr& b) {
			return a->GetCheckPointNum() < b->GetCheckPointNum();
		}
	);
	//���ꂼ��̃`�F�b�N�|�C���g�Ɍ��Ǝ��̃`�F�b�N�|�C���g���L�^����B
	for (int i = 0; i < listSize; i++)
	{
		//��O�̃`�F�b�N�|�C���g���L��������B
		m_checkPointList[i]->SetBackCheckPoint(m_checkPointList[(i + listSize - 1) % listSize].get());
		//���̃`�F�b�N�|�C���g���L��������B
		m_checkPointList[i]->SetNextCheckPoint(m_checkPointList[(i + 1) % listSize].get());
	}


	return true;
}

void CheckPointManager::Update()
{
	int checkPointSize = static_cast<int>(m_checkPointList.size());
	for (auto* controller : m_checkedControllerList) {
		int indexNo = controller->GetCheckPointNo();
		//�t�������H
		controller->SetIsReverseRun(m_checkPointList[indexNo]->CheckReverseRun(controller));
		//�`�F�b�N�|�C���g�̍X�V�����邩�H
		m_checkPointList[(indexNo + checkPointSize - 1) % checkPointSize]->CheckCharaconHit(controller);
		m_checkPointList[indexNo]->CheckCharaconHit(controller);
		m_checkPointList[(indexNo + 1) % checkPointSize]->CheckCharaconHit(controller);
	}
}

void CheckPointManager::OnDestroy()
{
}

void CheckPointManager::MakeCheckPoint(const Vector3& pos, const Quaternion& rot, int checkPointNum)
{
	//�`�F�b�N�|�C���g���쐬����B
	auto checkPoint = std::make_unique<CheckPointNormal>();
	checkPoint->SetPosition(pos);
	checkPoint->SetRotation(rot);
	checkPoint->SetCheckPointNum(checkPointNum);
	checkPoint->Init();
	//���X�g�ɐςݍ��ށB
	m_checkPointList.push_back(std::move(checkPoint));
}

void CheckPointManager::MakeGoalPoint(const Vector3& pos, const Quaternion& rot)
{
	//���ɂ���B
	if (m_goalPoint != nullptr)
	{
		ENGINE_LOG(
			"���ɃS�[���͑��݂��Ă��܂��B",
			" : ���x���t�@�C�����m�F�B")
		return;
	}
	//�S�[���̃`�F�b�N�|�C���g�����B
	auto goalPoint = std::make_unique<CheckPointGoal>();
	goalPoint->SetPosition(pos);
	goalPoint->SetRotation(rot);
	goalPoint->Init();
	//�S�[�������|�C���^���擾���Ă����B
	m_goalPoint = goalPoint.get();
	//���X�g�ɐςݍ��ށB
	m_checkPointList.push_back(std::move(goalPoint));
}
