#pragma once
class Car : public IGameObject
{
public:
	Car() {};
	~Car() {};
	bool Start() override;
	void Update() override;
	void OnDestroy() override;

public:		//Setä÷êîÇ∆Ç©
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}


private:
	prefab::ModelRender* m_model = nullptr;

	Vector3 m_position = Vector3::Zero;		//ç¿ïWÅB
};

