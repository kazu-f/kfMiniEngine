#include "stdafx.h"
#include "system/system.h"





const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの数。

/// <summary>
/// ディレクションライト。
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//ライトの方向。
	float pad0;			//パディング。
	Vector4 color;		//ライトのカラー。
};
/// <summary>
/// ライト構造体。
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//ディレクションライト。
	Vector3 eyePos;					//カメラの位置。
	float specPow;					//スペキュラの絞り。
	Vector3 ambinetLight;			//環境光。
};

#define USE_UNITY_CHAN 0
#define USE_NINJA 1
#define USE_ROBO 2

#define USE_MODEL 2


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ライトを用意する。
	Light light;
	
	light.directionalLight[0].color.x = 0.5f;
	light.directionalLight[0].color.y = 0.5f;
	light.directionalLight[0].color.z = 0.5f;
	
	light.directionalLight[0].direction.x = 0.0f;
	light.directionalLight[0].direction.y = 0.0f;
	light.directionalLight[0].direction.z = -1.0f;

	

	light.ambinetLight.x = 0.2f;
	light.ambinetLight.y = 0.2f;
	light.ambinetLight.z = 0.2f;
	light.eyePos = g_camera3D->GetPosition();
	light.specPow = 5.0f;

	//3Dモデルを作成。
	
	Model robotPBRModel, robotModel;
	ModelInitData initData;

	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	initData.m_fxFilePath = "Assets/shader/NoAnimModel_PBR.fx";
	initData.m_expandConstantBuffer = &light;
	initData.m_expandConstantBufferSize = sizeof(light);
	
	robotPBRModel.Init(initData);
	initData.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	robotModel.Init(initData);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 100.0f });
	Vector3 pos, scale;
	

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	robotPBRModel.UpdateWorldMatrix(pos, g_quatIdentity, scale);
	robotModel.UpdateWorldMatrix(pos, g_quatIdentity, scale);

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	bool isPBR = true;
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		
		
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		Quaternion qRot;
		if (g_pad[0]->IsPress(enButtonRight)) {
			qRot.SetRotationDegY(1.0f);
		}
		else if (g_pad[0]->IsPress(enButtonLeft)) {
			qRot.SetRotationDegY(-1.0f);
		}

		for (auto& lig : light.directionalLight) {
			qRot.Apply(lig.direction);
		}

		//カメラも回す。
		qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
		auto camPos = g_camera3D->GetPosition();
		qRot.Apply(camPos);
		g_camera3D->SetPosition(camPos);
		light.eyePos = g_camera3D->GetPosition();
		if (g_pad[0]->IsTrigger(enButtonA)) {
			isPBR = !isPBR;
		}
		if (isPBR) {
			//3Dモデルを表示する。
			robotPBRModel.Draw(renderContext);
		}
		else {
			robotModel.Draw(renderContext);
		}
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		//レンダリング終了。
		g_engine->EndFrame();
	}
	return 0;
}


