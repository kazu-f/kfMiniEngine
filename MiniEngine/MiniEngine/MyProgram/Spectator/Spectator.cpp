#include "stdafx.h"
#include "Spectator.h"

#define ON 1
#define OFF 0
#define ISBOX OFF			//デバッグ用に観客を箱に置き換えるか。

namespace {
	const float CULL_FAR = 20000.0f;
}

Spectator::Spectator()
{
}

Spectator::~Spectator()
{
}

bool Spectator::Start()
{
	//ファイルパス未設定。
	if (m_modelFilePath == nullptr)
	{
		ENGINE_MESSAGE_BOX(
			"観客のモデルのファイルパスが未設定。\n"
			"SetModelFilePath関数で指定してください。\n"
		);
		return false;
	}
	if (m_animFilePath == nullptr)
	{
		ENGINE_MESSAGE_BOX(
			"観客のアニメーションのファイルパスが未設定。\n"
			"SetAnimFilePath関数で指定してください。\n"
		);
		return false;
	}
	if (m_objectsData.size() <= 1)
	{
		ENGINE_MESSAGE_BOX(
			"観客のインスタンシング描画のデータが足りていない。\n"
			"インスタンシング描画には2個以上必要です。\n"
		);
		return false;
	}

	//モデル初期化。
	m_model = NewGO<prefab::ModelRender>(10);
	ModelInitData modelInitData;
#if ISBOX
	modelInitData.m_tkmFilePath = "Assets/modelData/testbg/testBox.tkm";	//デバッグ表示用の箱のモデル。
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";					//モデルシェーダー。
	modelInitData.m_vsEntryPointFunc = "VSMainInstancing";					//インスタンシング描画。

	//初期化。
	m_model->Init(
		modelInitData,
		nullptr,
		0,
		static_cast<int>(m_objectsData.size())
	);
#else
	modelInitData.m_tkmFilePath = m_modelFilePath;
	modelInitData.m_modelUpAxis = enModelUpAxis_Z;
	//アニメーション情報。
	AnimClipInitData animInitData[1];
	animInitData[0].tkaFilePath = m_animFilePath;
	animInitData[0].isLoop = true;
	//初期化。
	m_model->Init(
		modelInitData,
		animInitData,
		1,
		static_cast<int>(m_objectsData.size())
	);
#endif

	//影の設定。
	m_model->SetShadowCasterFlag(m_isShadowCaster);
	m_model->SetShadowReceiverFlag(false);
	m_model->SetCullingFar(CULL_FAR);

	return true;
}

void Spectator::Update()
{
	//リセット。
	m_model->BeginUpdateInstancingData();
	//インスタンシング描画用のデータを更新。
	for (auto& objData : m_objectsData) {
		m_model->UpdateInstancingData(objData.position, objData.rotation, objData.scale,true);
	}
}

void Spectator::OnDestroy()
{
	//削除。
	DeleteGO(m_model);
}
