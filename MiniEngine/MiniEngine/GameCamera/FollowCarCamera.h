#pragma once

class Car;

/// <summary>
/// �Ԃ�Ǐ]����J�����B
/// </summary>
class FollowCarCamera:public IGameObject
{
public:
	FollowCarCamera();					//�R���X�g���N�^�B
	~FollowCarCamera();					//�f�X�g���N�^�B
	bool Start()override final;			//�����������B
	void Update()override final;		//�X�V�����B
	void OnDestroy()override final;		//�폜�������B

public:
	//�Ǐ]����Ԃ��w�肷��B
	void SetTargetCar(Car* car)
	{
		m_car = car;
	}
	//

private:
	Car* m_car = nullptr;


};

