#pragma once
class CheckedController;
class LapUI;
class ReverseRunUI;

/// <summary>
/// レースを管理する。
/// </summary>
class RaceController : public IGameObject
{
public:
	RaceController();
	~RaceController();
	bool Start()override final;
	void Update()override final;
	void OnDestroy()override final;
	//プレイヤーを指定する。
	void SetPlayer(CheckedController* player)
	{
		m_player = player;
	}

private:
	CheckedController* m_player = nullptr;		//プレイヤー。
	LapUI* m_LapUI = nullptr;					//周回数の表示等を行うクラス。
	ReverseRunUI* m_reverseRunUI = nullptr;		//逆走中の表示を行うクラス。
};

