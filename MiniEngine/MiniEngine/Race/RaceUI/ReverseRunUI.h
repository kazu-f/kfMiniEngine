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


};

