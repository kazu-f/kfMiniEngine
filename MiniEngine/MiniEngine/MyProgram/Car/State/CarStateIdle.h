#pragma once
#include "ICarState.h"
class CarStateIdle : public ICarState
{
public:
	CarStateIdle(Car* car);			//コンストラクタ。
	~CarStateIdle();				//デストラクタ。
	//ドリフト可能か？
	bool IsDrift()override final
	{
		return false;
	}
	//減速中か？
	bool IsDecelerate() override final
	{
		return false;
	}

protected:
	void Enter()override final;		//事前処理。
	void Leave()override final;		//事後処理。
	void Execute()override final;	//実行処理。
private:
	prefab::CSoundSource* m_idleSound = nullptr;		//アイドリングの効果音。
};

