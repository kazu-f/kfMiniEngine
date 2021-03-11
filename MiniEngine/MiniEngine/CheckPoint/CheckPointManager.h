#pragma once

#include "CheckPointBase.h"
#include "CheckedController.h"
class CheckPointGoal;

/// <summary>
/// �`�F�b�N�|�C���g�̊Ǘ��ҁB
/// </summary>
class CheckPointManager : public IGameObject
{
public:
	CheckPointManager();
	~CheckPointManager();

	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	/// <summary>
	/// �`�F�b�N�|�C���g���쐬����B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="checkPointNum">�`�F�b�N�|�C���g�ԍ��B</param>
	void MakeCheckPoint(const Vector3& pos, const Quaternion& rot, int checkPointNum);
	/// <summary>
	/// �S�[�����쐬����B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="rot">��]�B</param>
	void MakeGoalPoint(const Vector3& pos, const Quaternion& rot);
	/// <summary>
	/// ���������J�n����B
	/// </summary>
	void EnableInit()
	{
		m_isStartInit = true;
	}

	/// <summary>
	/// �C���X�^���X���擾����B
	/// </summary>
	CheckPointManager* GetInstance()
	{
		return m_instance;
	}
	/// <summary>
	/// �`�F�b�N�R���g���[���[��o�^����B
	/// </summary>
	void RegistCheckedController(CheckedController* controller)
	{
		m_checkedControllerList.push_back(controller);
	}

private:
	static CheckPointManager* m_instance;

	typedef std::unique_ptr<CheckPointBase> CheckPointPtr;

	std::vector<CheckPointPtr> m_checkPointList;				//�`�F�b�N�|�C���g�̃��X�g�B
	std::vector<CheckedController*> m_checkedControllerList;	//�`�F�b�N�������̃��X�g�B
	CheckPointGoal* m_goalPoint = nullptr;					//�S�[���̃A�h���X�B
	bool m_isStartInit = false;								//�������J�n�t���O�B
};

