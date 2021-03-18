#pragma once

/// <summary>
/// ライティングのデモを行う。
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
	//カメラを回す。
	void RotCamera();
	//描画デモを変更。
	void ChangeDemo();

private:
	prefab::ModelRender* m_model = nullptr;				//モデル。
	std::vector<prefab::CDirectionLight*> m_lights;		//ライト。
	const Vector3 CAMERA_TARGETPOS = { 0.0f,50.0f,0.0f };
	Vector3 m_cameraPos = { 0.0f,0.0f,150.0f };		//カメラの座標。
	bool m_isRotCam = false;						//カメラの回転は有効か？
};

