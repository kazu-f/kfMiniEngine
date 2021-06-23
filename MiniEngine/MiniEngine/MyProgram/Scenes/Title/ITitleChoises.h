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
	//選択終了か？
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
		//スプライトの色をグレーにする。
		m_choisesSprite->SetMulColor(Vector4::Gray);
	}
	void SpriteColorWhite()
	{
		//スプライトの色を元の色にする。
		m_choisesSprite->SetMulColor(Vector4::White);
	}

protected:
	prefab::CSpriteRender* m_choisesSprite = nullptr;
	bool m_isSelected = false;			//選択完了か？
};

