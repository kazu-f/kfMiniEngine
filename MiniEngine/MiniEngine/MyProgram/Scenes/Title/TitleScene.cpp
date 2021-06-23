#include "stdafx.h"
#include "TitleScene.h"
#include "PickArrow.h"
#include "ITitleChoises.h"
#include "ChoiseGameStart.h"

namespace {
	const char* TITLE_SPRITE_PATH = "Assets/sprite/Title/Race_Title.dds";	//タイトルのファイルパス。
	//選択肢のスプライトのファイルパス。
	const char* CHOISES_PATH[TitleScene::EnChoises::en_ChoisesNum] = 
	{
		"Assets/sprite/Title/Race_GameStart.dds"
	};
	//選択肢の位置。
	const Vector3 CHOISES_POS[TitleScene::EnChoises::en_ChoisesNum] =
	{
		{200.0f,-150.0f,0.0f}
	};
	const Vector2 CHOISES_SPRITESIZE = { 500.0f,100.0f };		//選択肢のスプライトのサイズ。
	const Vector2 CHOISES_PIVOT= { 0.0f,0.5f };					//選択肢のスプライトのピボットの位置。
}

TitleScene::TitleScene()
{
	//ゲームスタートの選択肢。
	m_titleChoises[en_GameStart] = std::make_unique<ChoiseGameStart>(
		CHOISES_PATH[en_GameStart], CHOISES_SPRITESIZE);

	//選択肢のスプライトの座標等設定。
	for (int i = 0; i < EnChoises::en_ChoisesNum; i++)
	{
		//位置を合わせる。
		m_titleChoises[i]->SetPosition(CHOISES_POS[i]);
		m_titleChoises[i]->SetPivot(CHOISES_PIVOT);
	}
}

TitleScene::~TitleScene()
{
}

void TitleScene::OnDestroy()
{
	DeleteGO(m_titleSprite);
	DeleteGO(m_pickArrow);
}

bool TitleScene::Start()
{
	//タイトルのスプライト。
	m_titleSprite = NewGO<prefab::CSpriteRender>(5);
	m_titleSprite->Init(
		TITLE_SPRITE_PATH,
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);

	//矢印作成。
	m_pickArrow = NewGO<PickArrow>(0);
	m_pickArrow->SetPosition(CHOISES_POS[m_currentChoises]);

	return true;
}

void TitleScene::Update()
{
	m_pickArrow->SetPosition(CHOISES_POS[m_currentChoises]);

	if (Pad(0).IsTrigger(enButtonA))
	{
		m_pickArrow->ChangeSelectedState();
		m_isDecision = true;
	}

	if (m_isDecision) {
		m_titleChoises[m_currentChoises]->Execute();
	}

	if (m_titleChoises[m_currentChoises]->IsSelected())
	{
		DeleteGO(this);
	}
}

