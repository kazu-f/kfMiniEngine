#pragma once
class ITitleChoises
{
public:
	ITitleChoises(const char* filePath, const Vector2& size);
	virtual ~ITitleChoises();
	void Enter();
	void Leave();
	virtual void Execute() = 0;

public:
	//�I���I�����H
	bool IsSelected()
	{
		return m_isSelected;
	}

public:
	void SetPosition(const Vector3& pos)
	{
		m_choisesSprite->SetPosition(pos);
	}
	void SetPivot(const Vector2& pivot)
	{
		m_choisesSprite->SetPivot(pivot);
	}
protected:
	void SpriteColorGray()
	{
		//�X�v���C�g�̐F���O���[�ɂ���B
		m_choisesSprite->SetMulColor(Vector4::Gray);
	}
	void SpriteColorWhite()
	{
		//�X�v���C�g�̐F�����̐F�ɂ���B
		m_choisesSprite->SetMulColor(Vector4::White);
	}

protected:
	prefab::CSpriteRender* m_choisesSprite = nullptr;
	bool m_isSelected = false;			//�I���������H
};

