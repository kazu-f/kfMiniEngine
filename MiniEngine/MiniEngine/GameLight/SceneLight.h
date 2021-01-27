#pragma once
class SceneLight:public IGameObject
{
public:
	SceneLight();
	~SceneLight();
	bool Start() override final;

private:
	std::vector<prefab::CDirectionLight*> m_lightArray;
	Vector3 m_lightDir = Vector3::Zero;

};

