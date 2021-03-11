#pragma once
/// <summary>
/// �Q�[���N���X�B
/// </summary>

class FollowCarCamera;
class Car;
class Spectator;
class Guardrail;
class SceneLight;
class CheckPointManager;

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//�������B
	void Release();		//����������K�v�Ȃ炱��B
	void PreUpdate()override;
	void Update();		//�X�V�����B

private:
	//�������X�e�b�v�B
	enum EnInitStep {
		enInit_Other,
		enInit_Course,
		enInit_Spectator,
		enInit_CheckPoint,
	};
	//�ϋq�̗񋓁B
	enum EnSpectator {
		enShirtMale,
		enNathanMale,
		enSophiaWoman,
		enClaudiaWoman,
		enSpectatorNum
	};
	EnInitStep m_initState = enInit_Other;	//�������X�e�b�v�B

	CLevel m_courseLevel;					//�R�[�X�̃��x���B
	CLevel m_spectatorLevel;				//�ϋq�̃��x���B
	CheckPointManager* m_checkPointManager = nullptr;	//�`�F�b�N�|�C���g�̊Ǘ��ҁB
	FollowCarCamera* m_camera = nullptr;		//�J�����B
	Car* m_car = nullptr;					//�ԁB
	Spectator* m_spectator[enSpectatorNum] = { nullptr };		//�ϋq�B
	Guardrail* m_guardrail = nullptr;		//�K�[�h���[���B
	SceneLight* m_light = nullptr;			//�Ɩ��B
};

