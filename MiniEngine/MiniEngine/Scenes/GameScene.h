#pragma once
/// <summary>
/// �Q�[���N���X�B
/// </summary>

class CGameCamera;
class Car;
class Spectator;
class SceneLight;

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
		enInit_Athor,
		enInit_Course,
		enInit_Spectator,
	};
	//�ϋq�̗񋓁B
	enum EnSpectator {
		enShirtMale,
		enNathanMale,
		enSophiaWoman,
		enClaudiaWoman,
		enSpectatorNum
	};
	EnInitStep m_initState = enInit_Athor;	//�������X�e�b�v�B

	CLevel m_courseLevel;					//�R�[�X�̃��x���B
	CLevel m_spectatorLevel;				//�ϋq�̃��x���B
	CGameCamera* m_camera = nullptr;		//�J�����B
	Car* m_car = nullptr;					//�ԁB
	Spectator* m_spectator[enSpectatorNum] = { nullptr };		//�ϋq�B
	SceneLight* m_light = nullptr;			//�Ɩ��B
};

