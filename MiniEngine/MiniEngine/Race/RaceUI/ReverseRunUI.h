#pragma once

/// <summary>
/// �t����UI
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
	/// �t�����Ă��邩��ݒ肷��B
	/// </summary>
	/// <param name="flag">�t���O�B(true�Ȃ�t�����B)</param>
	void SetIsReverseRunFlag(bool flag)
	{
		m_isReverseRun = flag;
	}

private:
	void ReverseRun();

private:
	prefab::CSpriteRender* m_sprite = nullptr;

	float m_currentTime = 0.0f;
	bool m_isReverseRun = false;
};

