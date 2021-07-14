#pragma once
class Spectator;
class CGameCamera;

//�J�����O�̃e�X�g���s���B
class CullingTestScene : public IGameObject
{
public:
	CullingTestScene();
	~CullingTestScene();
	bool Start();
	void OnDestroy();
	void Update();

private:
	Spectator* m_spectator = nullptr;		//�C���X�^���V���O�`����s���N���X�B
	CGameCamera* m_camera = nullptr;			//�J�����B
	CLevel m_level;							//���x���ǂݍ��݁B
	prefab::CDirectionLight* m_light = nullptr;
};

