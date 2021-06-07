#pragma once
/// <summary>
/// �Q�[���N���X�B
/// </summary>
#include "Spectator/SpectatorNames.h"

class FollowCarCamera;
class Car;
class Spectator;
class Guardrail;
class SceneLight;
class CheckPointManager;
class BackGround;
class RaceController;

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//�������B
	void Release();		//����������K�v�Ȃ炱��B
	void PreUpdate()override;
	void Update();		//�X�V�����B
	void OnDestroy()override final;	//�폜�������B

private:	//�������X�e�b�v�ŌĂяo�������B
	void InitOther();			//�J�����⃉�C�g���������B
	void InitCourse();			//�R�[�X��ǂݍ��ށB
	void InitSpectator();		//�ϋq��z�u����B
	void InitRaceController();	//���[�X�̐�����s���N���X�̏������B
	void InitCheckPoint();		//�`�F�b�N�|�C���g��z�u����B

private:
	/// <summary>
	/// �ϋq�̃f�[�^��ǉ����Ă����B
	/// </summary>
	/// <param name="objData">�I�u�W�F�N�g�̃f�[�^�B</param>
	/// <param name="type">�ϋq�̎��</param>
	void AddSpectatorData(SLevelObjectData& objData,int type);

private:
	//�������X�e�b�v�B
	enum EnInitStep {
		enInit_Other,				//���C�g��J�����̍쐬�B
		enInit_Course,				//�R�[�X�̓ǂݍ��݁B
		enInit_Spectator,			//�ϋq�̔z�u�B
		enInit_RaceController,		//���[�X����N���X�̍쐬�B
		enInit_CheckPoint,			//�`�F�b�N�|�C���g�̔z�u�B
		enInit_End,					//�������X�e�b�v�I���B
	};
	EnInitStep m_initState = enInit_Other;	//�������X�e�b�v�B

	CLevel m_courseLevel;					//�R�[�X�̃��x���B
	CLevel m_spectatorLevel;				//�ϋq�̃��x���B
	CheckPointManager* m_checkPointManager = nullptr;	//�`�F�b�N�|�C���g�̊Ǘ��ҁB
	RaceController* m_raceController = nullptr;			//���[�X�̊Ǘ��B
	FollowCarCamera* m_camera = nullptr;		//�J�����B
	Car* m_playerCar = nullptr;					//�ԁB
	Car* m_cpuCar = nullptr;					//�ԁB
	BackGround* m_ground = nullptr;			//�n�ʁB
	Spectator* m_spectator[enSpectatorNum] = { nullptr };		//�ϋq�B
	Guardrail* m_guardrail = nullptr;		//�K�[�h���[���B
	SceneLight* m_light = nullptr;			//�Ɩ��B
	prefab::CSky* m_sky = nullptr;			//��B
};

