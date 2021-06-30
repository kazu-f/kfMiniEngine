#include "stdafx.h"
#include "PickArrow.h"

namespace {
	const char* PICK_PATH = "Assets/sprite/Title/Race_ChoisePick.dds";		//���̃X�v���C�g�B
	const Vector2 PICK_SIZE = { 100.0f,50.0f };				//���̃X�v���C�g�̃T�C�Y�B
	const Vector2 PICK_PIVOT = { 1.0f,0.5f };					//���̃X�v���C�g�̃s�{�b�g�̈ʒu�B
}

PickArrow::PickArrow()
{
}

PickArrow::~PickArrow()
{
}

void PickArrow::OnDestroy()
{
	DeleteGO(m_pickSprite);
}

bool PickArrow::Start()
{
	//���̃X�v���C�g�B
	m_pickSprite = NewGO<prefab::CSpriteRender>(5);
	m_pickSprite->Init(
		PICK_PATH,
		PICK_SIZE.x,
		PICK_SIZE.y
	);
	m_pickSprite->SetPivot(PICK_PIVOT);			//�s�{�b�g�̈ʒu�𒲐��B
	m_pickSprite->SetPosition(m_position);	//�I���������킹��B

	return true;
}

void PickArrow::Update()
{
	//���Ԃ��o�߂�����B
	m_swingTime += GameTime().GetFrameDeltaTime() * SWING_SPEED[m_state];
	float t = sinf(m_swingTime) / 2.0f + 0.5f;		//sin�g��0.0�`1.0�ɕ⊮���ėh�炷�B
	m_pickSprite->SetPosition(m_position + m_pickSwing * t);
}
