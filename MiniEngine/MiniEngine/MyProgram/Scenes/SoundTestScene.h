#pragma once
class SoundTestScene : public IGameObject
{
public:
	SoundTestScene();
	~SoundTestScene();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;

private:
	void MoveCamera();
	void MoveSound();

private:
	prefab::ModelRender* m_ground = nullptr;
	prefab::ModelRender* m_model = nullptr;
	prefab::CSoundSource* m_sound = nullptr;
	Vector3 m_camPos = Vector3::Zero;
	Vector3 m_soundPos = Vector3::Zero;
	float m_pitch = 1.0f;
	bool m_moveFlag = false;
};

