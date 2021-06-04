#include "stdafx.h"
#include "LapUI.h"

namespace {
	const Vector3 TEXTPOS = { 360.0f,-320.0f,0.0f };				//�e�L�X�g�̈ʒu�B
	const Vector2 TEXTSIZE = { 200.0f,100.0f };				//�e�L�X�g�̃T�C�Y�B

	//���񐔂̃X�v���C�g�̃t�@�C���p�X�B
	const char* LAPNUM_FILEPATH[LapUI::enLap_Num] = {
		"Assets/sprite/LapNum_1.dds",
		"Assets/sprite/LapNum_2.dds",
		"Assets/sprite/LapNum_3.dds"
	};
	const char* LAPTEXT_FILEPATH = "Assets/sprite/LAP.dds";				//�e�L�X�g�������ꂽ�X�v���C�g(LAP)
	const char* LAPSLASH_FILEPATH = "Assets/sprite/LapSlash.dds";		//�X���b�V��(/)�̃X�v���C�g�B

	const Vector3 CURRENT_LAPPOS = { 480.0f,-320.0f,0.0f };		//���݂̎��񐔂̈ʒu�B
	const Vector3 MAX_LAPPOS = { 580.0f,-320.0f,0.0f };			//�ő���񐔂̈ʒu�B
	const Vector3 SLASH_POS = { 530.0f,-320.0f,0.0f };			//�X���b�V���̈ʒu�B
	const Vector2 LAPNUM_SIZE = { 100.0f,100.0f };			//���񐔂̃T�C�Y�B

	const int SPRITE_PRIO = 5;									//�X�v���C�g�̎��s�D��x�B

	const float INTARVAL_TIME = 0.4f;						//�_�ŊԊu���ԁB
	const float MAX_TIME = 2.0f;							//�_�Ŏ��ԁB
}

LapUI::LapUI()
{
}

LapUI::~LapUI()
{
}

void LapUI::OnDestroy()
{
	//�㏈���폜�B
	DeleteGO(m_lapTextSprite);
	DeleteGO(m_maxLapSprite);
	DeleteGO(m_slashSprite);
	for (int i = 0; i < enLap_Num; i++) {
		DeleteGO(m_currentLapSprite[i]);
	}
}

bool LapUI::Start()
{
	//�e�L�X�g�B
	m_lapTextSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_lapTextSprite->Init(LAPTEXT_FILEPATH, TEXTSIZE.x, TEXTSIZE.y);
	m_lapTextSprite->SetPosition(TEXTPOS);
	//�X���b�V���B
	m_slashSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_slashSprite->Init(LAPSLASH_FILEPATH, LAPNUM_SIZE.x, LAPNUM_SIZE.y);
	m_slashSprite->SetPosition(SLASH_POS);
	//�ő���񐔁B
	m_maxLapSprite = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
	m_maxLapSprite->Init(LAPNUM_FILEPATH[enLap_Num - 1], LAPNUM_SIZE.x, LAPNUM_SIZE.y);
	m_maxLapSprite->SetPosition(MAX_LAPPOS);
	//���ݎ��񐔁B
	for (int i = 0; i < enLap_Num; i++) {
		m_currentLapSprite[i] = NewGO<prefab::CSpriteRender>(SPRITE_PRIO);
		m_currentLapSprite[i]->Init(LAPNUM_FILEPATH[i], LAPNUM_SIZE.x, LAPNUM_SIZE.y);
		m_currentLapSprite[i]->SetPosition(CURRENT_LAPPOS);
		m_currentLapSprite[i]->SetActiveFlag(false);
	}
	//���ݎ��񐔂����L���ɂ���B
	m_currentLapSprite[m_currentLapNum]->SetActiveFlag(true);

	return true;
}

void LapUI::Update()
{
	//���񐔂������Ă��Ȃ��B
	if (!(m_currentLapNum < m_nextLapNum)) {
		return;
	}
	if (m_nextLapNum >= enLap_Num) {
		//�S�[�������B
		m_isGoal = true;
	}
	else {
		//���񐔂�؂�ւ���B�B
		CountUpLap();
	}

}

void LapUI::CountUpLap()
{
	m_currentLapSprite[m_currentLapNum]->SetActiveFlag(false);

	//���Ԍo�߁B
	m_currentTime += GameTime().GetFrameDeltaTime();

	if (m_currentTime > MAX_TIME) {
		m_currentLapSprite[m_nextLapNum]->SetActiveFlag(true);
		m_currentLapNum = m_nextLapNum;	//�J�E���g���グ��B
		m_currentTime = 0.0f;			//���ɖ߂��B
	}
	else {
		//���Ԃ��Ԋu�Ŋ���B
		float flagTime = m_currentTime / INTARVAL_TIME;
		//�����������߂�B
		int count = static_cast<int>(floorf(flagTime));
		//�������̋�����ŃA�N�e�B�u�t���O��؂�ւ��B
		int activeFlag = count % 2;
		m_currentLapSprite[m_nextLapNum]->SetActiveFlag(activeFlag == 1);
	}
}
