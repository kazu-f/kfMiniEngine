#pragma once
#include "ICarState.h"
class CarStateIdle : public ICarState
{
public:
	CarStateIdle(Car* car);			//�R���X�g���N�^�B
	~CarStateIdle();				//�f�X�g���N�^�B
	//�h���t�g�\���H
	bool IsDrift()override final
	{
		return false;
	}
	//���������H
	bool IsDecelerate() override final
	{
		return false;
	}

protected:
	void Enter()override final;		//���O�����B
	void Leave()override final;		//���㏈���B
	void Execute()override final;	//���s�����B
private:
	prefab::CSoundSource* m_idleSound = nullptr;		//�A�C�h�����O�̌��ʉ��B
};

