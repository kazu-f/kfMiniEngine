#include "stdafx.h"
#include "LightingDemoScene.h"

#define ROTMODE 0
#define AUTOMODE 1

#define CAMERA_ROTMODE AUTOMODE

namespace {
	//モデルのファイルパス。
	const char* MODEL_FILEPATH[] = {
		"Assets/modelData/unityChan2.tkm",
	};
	//デモモデルの列挙。
	enum EnDemoModel {
		enDemo_Unity,
		enDemo_Num
	};


	//ライトデータ。
	struct LightData
	{
		Vector3 Dir = Vector3::Zero;
		Vector4 Color = Vector4::White;
	};

	const int LIGHT_NUM = 2;						//ライトの本数。
	const float LIGHT_POW = 2.0f;					//ライトの強さ。
	//ライトデータ。
	const LightData LIGHTDATAS[LIGHT_NUM] = {
		{ {1.0f,0.0f,0.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {1.0f,0.0f,0.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
	};

	//カメラ
	const float CAMERA_ROTSPEED = 90.0f;
	const float AUTO_ROTSPEED = 0.8f;

	const char* DEMO_SHADER = "Assets/shader/modelDemo.fx";
	const char* MODEL_SHADER = Model::MODEL_SHADER_PAHT;

	struct DemoConstantBuffer {
		int isPBR = 0;		//PBR
		int isNormal = 0;	//法線
		int isSpec = 0;		//スペキュラ。
	};
	DemoConstantBuffer DEMO_CB = { 0,0,0 };
	
	enum EnDemoState {
		enNormal,		//法線。
		enSpec,			//スペキュラ。
		enPBR,			//PBR。
		enDemoNum
	};

	int DEMO_STATE = 0;

}

LightingDemoScene::LightingDemoScene()
{
}

LightingDemoScene::~LightingDemoScene()
{
}

bool LightingDemoScene::Start()
{
	m_model = NewGO<prefab::ModelRender>(0);

	ModelInitData modelData;
	modelData.m_fxFilePath = DEMO_SHADER;
	modelData.m_tkmFilePath = MODEL_FILEPATH[enDemo_Unity];
	modelData.m_expandConstantBuffer = &DEMO_CB;
	modelData.m_expandConstantBufferSize = sizeof(DemoConstantBuffer);
	
	m_model->Init(modelData);
	m_model->SetForwardRenderFlag(true);

	for (int i = 0; i < LIGHT_NUM; i++)
	{
		prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);
		light->SetDirection(LIGHTDATAS[i].Dir);
		light->SetColor(LIGHTDATAS[i].Color);
		m_lights.push_back(light);
	}

	MainCamera().SetTarget(CAMERA_TARGETPOS);
	MainCamera().SetPosition(CAMERA_TARGETPOS + m_cameraPos);

	return true;
}

void LightingDemoScene::Update()
{
	RotCamera();
	ChangeDemo();
}

void LightingDemoScene::OnDestroy()
{
	DeleteGO(m_model);
}

void LightingDemoScene::RotCamera()
{
#if CAMERA_ROTMODE
	const float PadX = AUTO_ROTSPEED;
#else
	const float PadX = Pad(0).GetRStickXF();
#endif
	const float PadY = Pad(0).GetRStickYF();
	
	const float delTime = GameTime().GetFrameDeltaTime();

#if CAMERA_ROTMODE
	if (Pad(0).IsTrigger(enButtonA))
	{
		m_isRotCam = true;
	}
#else
	if (PadX > 0.01f ||
		PadY > 0.01f)
	{
		m_isRotCam = true;
	}
#endif
	if (!m_isRotCam)return;

	//カメラの位置を決める。

	//X回転。
	Quaternion qRotX;
	qRotX.SetRotationDegY(PadX * CAMERA_ROTSPEED * delTime);

	qRotX.Apply(m_cameraPos);
	
	//Y回転。
	Vector3 cameraToTarget = CAMERA_TARGETPOS - m_cameraPos;
	cameraToTarget.Normalize();
	Vector3 axisRotY = MainCamera().GetRight();
	Quaternion qRotY;
	qRotY.SetRotationDeg(axisRotY, PadY * CAMERA_ROTSPEED * delTime);

	qRotY.Apply(m_cameraPos);

	MainCamera().SetPosition(CAMERA_TARGETPOS + m_cameraPos);
}

void LightingDemoScene::ChangeDemo()
{
	//LB1で切り替え。キーボードだと[B]キーに割り当てられている。
	if (Pad(0).IsTrigger(enButtonLB1)) {
		//ステートで切り替える。
		switch (DEMO_STATE)
		{
		case enPBR:
			//切り替え。
			DEMO_CB.isPBR = DEMO_CB.isPBR ^ 1;
			break;
		case enNormal:
			//切り替え。
			DEMO_CB.isNormal = DEMO_CB.isNormal ^ 1;
			break;
		case enSpec:
			//切り替え。
			DEMO_CB.isSpec = DEMO_CB.isSpec ^ 1;
			break;
		default:
			break;
		}
		DEMO_STATE++;
		DEMO_STATE %= enDemoNum;
	}
}
