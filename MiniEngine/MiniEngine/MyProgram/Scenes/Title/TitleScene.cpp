#include "stdafx.h"
#include "TitleScene.h"
#include "PickArrow.h"
#include "ITitleChoises.h"
#include "ChoiseGameStart.h"

namespace {
	const char* TITLE_SPRITE_PATH = "Assets/sprite/Title/Race_Title.dds";	//�^�C�g���̃t�@�C���p�X�B
	const char* PRESSA_SPRITE_PATH = "Assets/sprite/Title/PressA.dds";	//PressA�\���̃t�@�C���p�X�B
	Vector2 PRESSA_SIZE = { 300.0f,75.0f };							//PressA�\���̃T�C�Y�B
	Vector2 PRESSA_POS = { -360.0f,-260.0f };							//PressA�\���̈ʒu�B

	//�I�����̃X�v���C�g�̃t�@�C���p�X�B
	const char* CHOISES_PATH[TitleScene::EnChoises::en_ChoisesNum] = 
	{
		"Assets/sprite/Title/Race_GameStart.dds"
	};
	//�I�����̈ʒu�B
	const Vector3 CHOISES_POS[TitleScene::EnChoises::en_ChoisesNum] =
	{
		{200.0f,-150.0f,0.0f}
	};

	const wchar_t* BGM_FILEPATH = L"Assets/sound/Title/RaceGameTitle.wav";		//�^�C�g��BGM�B
	const float BGM_VOLUME = 0.5f;		//BGM�̃{�����[���B

	const Vector2 CHOISES_SPRITESIZE = { 500.0f,100.0f };		//�I�����̃X�v���C�g�̃T�C�Y�B
	const Vector2 CHOISES_PIVOT= { 0.0f,0.5f };					//�I�����̃X�v���C�g�̃s�{�b�g�̈ʒu�B
}

TitleScene::TitleScene()
{
	//�Q�[���X�^�[�g�̑I�����B
	m_titleChoises[en_GameStart] = std::make_unique<ChoiseGameStart>(
		CHOISES_PATH[en_GameStart], CHOISES_SPRITESIZE);

	//�I�����̃X�v���C�g�̍��W���ݒ�B
	for (int i = 0; i < EnChoises::en_ChoisesNum; i++)
	{
		//�ʒu�����킹��B
		m_titleChoises[i]->SetPosition(CHOISES_POS[i]);
		m_titleChoises[i]->SetPivot(CHOISES_PIVOT);
	}
}

TitleScene::~TitleScene()
{
	DeleteGO(m_titleSprite);
	DeleteGO(m_pressA);
	DeleteGO(m_pickArrow);
}

void TitleScene::OnDestroy()
{
	DeleteGO(m_titleBGM);
}

bool TitleScene::Start()
{
	//�^�C�g���̃X�v���C�g�B
	m_titleSprite = NewGO<prefab::CSpriteRender>(5);
	m_titleSprite->Init(
		TITLE_SPRITE_PATH,
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
	
	//�^�C�g���̃X�v���C�g�B
	m_pressA = NewGO<prefab::CSpriteRender>(6);
	m_pressA->Init(
		PRESSA_SPRITE_PATH,
		PRESSA_SIZE.x,
		PRESSA_SIZE.y
	);
	m_pressA->SetPosition(PRESSA_POS);
	m_pressA->SetPivot(Sprite::DEFAULT_PIVOT);

	//���쐬�B
	m_pickArrow = NewGO<PickArrow>(0);
	m_pickArrow->SetPosition(CHOISES_POS[m_currentChoises]);

	//BGM�𗬂��B
	m_titleBGM = NewGO<prefab::CSoundSource>(0);
	m_titleBGM->InitStreaming(BGM_FILEPATH);
	m_titleBGM->Play(true);
	m_titleBGM->SetVolume(BGM_VOLUME * CFade::GetInstance()->GetFadeRate());

	CFade::GetInstance()->StartFadeIn();

	return true;
}

void TitleScene::Update()
{
	auto* fade = CFade::GetInstance();
	//�{�����[�����t�F�[�h�ɍ��킹�ĕω��B
	m_titleBGM->SetVolume(BGM_VOLUME * fade->GetFadeRate());

	m_currentTime += GameTime().GetFrameDeltaTime();
	float alpha = sinf(m_currentTime * 2.0f);
	alpha = alpha * 0.5f + 0.5f;
	m_pressA->SetMulColor({ 1.0f,1.0f,1.0f,alpha });

	if (fade->IsFade()) return;

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

