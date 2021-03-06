#pragma once

/// <summary>
/// 逆走のUI
/// </summary>
class ReverseRunUI : public IGameObject
{
public:
	ReverseRunUI();
	~ReverseRunUI();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;

	/// <summary>
	/// 逆走しているかを設定する。
	/// </summary>
	/// <param name="flag">フラグ。(trueなら逆走中。)</param>
	void SetIsReverseRunFlag(bool flag)
	{
		m_isReverseRun = flag;
	}

private:
	void ReverseRun();

private:
	prefab::CSpriteRender* m_sprite = nullptr;	//スプライト。

	float m_currentTime = 0.0f;					//現在の経過時間。
	bool m_isReverseRun = false;				//逆走中か？
};

