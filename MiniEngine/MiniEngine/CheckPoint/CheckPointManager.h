#pragma once

#include "CheckPointBase.h"
#include "CheckedController.h"

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

private:
	typedef std::unique_ptr<CheckPointBase> CheckPointPtr;

	std::list<CheckPointPtr> m_checkPointList;				//�`�F�b�N�|�C���g�̃��X�g�B
	std::vector<CheckedController*> m_checkdControllerList;	//�`�F�b�N�������̃��X�g�B
};

