#include "stdafx.h"
#include "LightingDemoScene.h"

namespace {
	//���f���̃t�@�C���p�X�B
	const char* MODEL_FILEPATH[] = {
		"Assets/modelData/unityChan2.tkm",
	};
	//�f�����f���̗񋓁B
	enum EnDemoModel {
		enDemo_Unity,
		enDemo_Num
	};


	//���C�g�f�[�^�B
	struct LightData
	{
		Vector3 Dir = Vector3::Zero;
		Vector4 Color = Vector4::White;
	};

	const int LIGHT_NUM = 2;						//���C�g�̖{���B
	//���C�g�f�[�^�B
	const LightData LIGHTDATAS[LIGHT_NUM] = {
		{ {1.0f,0.0f,0.0f},Vector4::White * 5.0f },
		{ {1.0f,0.0f,0.0f},Vector4::White * 5.0f },
	};

	//�J����
	const float CAMERA_ROTSPEED = 90.0f;


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
	modelData.m_fxFilePath = "Assets/shader/model.fx";
	modelData.m_tkmFilePath = MODEL_FILEPATH[enDemo_Unity];
	
	m_model->Init(modelData);

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
}

void LightingDemoScene::OnDestroy()
{
	DeleteGO(m_model);
}

void LightingDemoScene::RotCamera()
{
	const float PadX = g_pad[0]->GetRStickXF();
	const float PadY = g_pad[0]->GetRStickYF();
	const float delTime = GameTime().GetFrameDeltaTime();


	//�J�����̈ʒu�����߂�B

	//X��]�B
	Quaternion qRotX;
	qRotX.SetRotationDegY(PadX * CAMERA_ROTSPEED * delTime);
	qRotX.Apply(m_cameraPos);
	
	//Y��]�B
	Vector3 cameraToTarget = CAMERA_TARGETPOS - m_cameraPos;
	cameraToTarget.Normalize();
	Vector3 axisRotY;
	//�ق�Y���B
	if (fabsf(cameraToTarget.Dot(Vector3::AxisY)) > 0.9998 ) {
		axisRotY.Cross(cameraToTarget,Vector3::AxisZ);
	}
	else {
		axisRotY.Cross(cameraToTarget, Vector3::AxisY);
	}
	Quaternion qRotY;
	qRotY.SetRotationDeg(axisRotY, PadY * CAMERA_ROTSPEED * delTime);

	qRotY.Apply(m_cameraPos);

	MainCamera().SetPosition(CAMERA_TARGETPOS + m_cameraPos);
}
