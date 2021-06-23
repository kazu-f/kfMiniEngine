#pragma once
class PickArrow : public IGameObject
{
public:
	PickArrow();
	~PickArrow();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;
public:
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	//決定済み。
	void ChangeSelectedState()
	{
		m_state = en_Selected;
	}
private:
	enum EnPickState {
		en_Idle,
		en_Selected,
		en_PickStateNum
	};
	const float SWING_SPEED[en_PickStateNum] = { 3.0f ,12.0f};

private:
	EnPickState m_state = en_Idle;
	prefab::CSpriteRender* m_pickSprite = nullptr;		//ピック中の選択肢を指すスプライト。
	Vector3 m_position = Vector3::Zero;					//座標。
	Vector3 m_pickSwing = { -10.0f,0.0f,0.0f };			//矢印を揺らす幅。
	float m_swingTime = 0.0f;							//経過時間。
};

