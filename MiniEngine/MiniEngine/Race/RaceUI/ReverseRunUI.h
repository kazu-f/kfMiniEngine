#pragma once

/// <summary>
/// ‹t‘–‚ÌUI
/// </summary>
class ReverseRunUI : public IGameObject
{
public:
	ReverseRunUI();
	~ReverseRunUI();
	bool Start() override final;
	void Update() override final;
	void OnDestroy() override final;


};

