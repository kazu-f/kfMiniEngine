#pragma once

/// <summary>
/// ���C�e�B���O�̃f�����s���B
/// </summary>

class LightingDemoScene : public IGameObject
{
public:
	LightingDemoScene();
	~LightingDemoScene();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;

private:
	//�J�������񂷁B
	void RotCamera();
	//�`��f����ύX�B
	void ChangeDemo();

private:
	prefab::ModelRender* m_model = nullptr;				//���f���B
	std::vector<prefab::CDirectionLight*> m_lights;		//���C�g�B
	const Vector3 CAMERA_TARGETPOS = { 0.0f,50.0f,0.0f };
	Vector3 m_cameraPos = { 0.0f,0.0f,150.0f };		//�J�����̍��W�B
	bool m_isRotCam = false;						//�J�����̉�]�͗L�����H
};

