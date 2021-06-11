#include "stdafx.h"
#include "LightingDemoScene.h"

#define ROTMODE 0
#define AUTOMODE 1

#define CAMERA_ROTMODE ROTMODE

#define SKY_OFF

namespace {
	//モデルのファイルパス。
	const char* MODEL_FILEPATH[] = {
		"Assets/modelData/unityChan2.tkm",
		"Assets/modelData/Car/Car.tkm",
	};
	//デモモデルの列挙。
	enum EnDemoModel {
		enDemo_Unity,
		enDemo_Car,
		enDemo_Num
	};


	//ライトデータ。
	struct LightData
	{
		Vector3 Dir = Vector3::Zero;
		Vector4 Color = Vector4::White;
	};

	const int LIGHT_NUM = 5;						//ライトの本数。
	//ライトデータ。
#ifdef SKY_ON
	const float AMBIENT = 0.5f;						//アンビエントライト。
	const float LIGHT_POW = 0.6;					//ライトの強さ。
	const LightData LIGHTDATAS[LIGHT_NUM] = {
		{ {0.0f,-1.0f,0.0f},{1.5f,1.5f,1.5f,1.0f } },
		{ {-1.0f,-1.0f,0.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {0.0f,-1.0f,1.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {0.0f,-1.0f,-1.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {1.0f,-1.0f,0.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
	};
#else
	const float AMBIENT = 0.3f;						//アンビエントライトの強さ。
	const float LIGHT_POW = 0.2f;					//ライトの強さ。
	const LightData LIGHTDATAS[LIGHT_NUM] = {
		{ {0.0f,-1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f } },
		{ {-1.0f,-1.0f,0.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {0.0f,-1.0f,1.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {0.0f,-1.0f,-1.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
		{ {1.0f,-1.0f,0.0f},{LIGHT_POW,LIGHT_POW,LIGHT_POW,1.0f } },
	};
#endif // SKY_ON


	//カメラ
	const float CAMERA_ROTSPEED = 90.0f;
	const float AUTO_ROTSPEED = 0.8f;

	const wchar_t* DEMO_SHADER = L"Assets/shader/modelDemo.fx";
	const wchar_t* MODEL_SHADER = CPipelineStatesDefault::MODEL_SHADER_PATH;

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
	//modelData.m_shaderData.vsFxFilePath = MODEL_SHADER;
	//modelData.m_shaderData.vsEntryPointFunc = "VSMain";
	//modelData.m_shaderData.psFxFilePath = MODEL_SHADER;
	//modelData.m_shaderData.psEntryPointFunc = "PSMain";
	modelData.m_tkmFilePath = MODEL_FILEPATH[enDemo_Car];
	modelData.m_expandConstantBuffer = &DEMO_CB;
	modelData.m_expandConstantBufferSize = sizeof(DemoConstantBuffer);
	
	m_model->Init(modelData);
	m_model->SetScale(Vector3::One * 0.3f);
	m_model->SetPosition({ 0.0f,30.0f,0.0f });
	//m_model->SetForwardRenderFlag(true);

	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);
	light->SetDirection({0.5f,-1.0f,-0.5f});
	light->SetColor(LIGHTDATAS[0].Color);
	m_lights.push_back(light);

	//for (int i = 0; i < LIGHT_NUM; i++)
	//{
	//	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);
	//	light->SetDirection(LIGHTDATAS[i].Dir);
	//	light->SetColor(LIGHTDATAS[i].Color);
	//	m_lights.push_back(light);
	//}

	MainCamera().SetTarget(CAMERA_TARGETPOS);
	MainCamera().SetPosition(CAMERA_TARGETPOS + m_cameraPos);
	MainCamera().SetFar(200000.0f);

#ifdef SKY_ON
	//空を付けておく。
	const float SkySize = 1000.0f;
	m_sky = NewGO<prefab::CSky>(0);
	m_sky->SetScale(SkySize);
	const float SKY_EMISSION = 0.05f;
	m_sky->SetEmissionColor({ SKY_EMISSION ,SKY_EMISSION ,SKY_EMISSION });

#endif // SKY_ON

	GraphicsEngine()->GetPostEffect()->GetTonemap().SetLuminance(0.54f);
	GraphicsEngine()->GetLightManager()->SetAmbientLight({ AMBIENT ,AMBIENT ,AMBIENT });

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

#ifdef SKY_ON
	if (Pad(0).IsTrigger(enButtonX))
	{
		//CreateLight(LIGHTDATAS[0].Dir, LIGHTDATAS[0].Color);
		m_skyEmission = m_skyEmission + Vector3::One * 0.1f;
		m_sky->SetEmissionColor(m_skyEmission);
	}
	if (Pad(0).IsTrigger(enButtonY))
	{
		//DeleteLight();
		m_skyEmission = m_skyEmission - Vector3::One * 0.1f;
		m_sky->SetEmissionColor(m_skyEmission);
	}
#endif // SKY_ON

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

void LightingDemoScene::CreateLight(const Vector3& dir, const Vector4& col)
{
	prefab::CDirectionLight* light = NewGO<prefab::CDirectionLight>(0);
	light->SetDirection(dir);
	light->SetColor(col);
	m_lights.push_back(light);
}

void LightingDemoScene::DeleteLight()
{
	if (m_lights.size() == 0)return;
	//先頭のライトを消す。
	DeleteGO(*m_lights.begin());
	//ライトの登録を解除。
	m_lights.erase(m_lights.begin());
}
