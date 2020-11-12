#pragma once
class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};
	bool Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	enum EnAnimState
	{
		en_animIdle,
		en_animRun,
		en_animNum
	};

	prefab::ModelRender* m_model = nullptr;
};

