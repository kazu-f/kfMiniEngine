#pragma once

#include "ICarState.h"

/// <summary>
/// アクセルステート。
/// </summary>
class CarStateAccele : public ICarState
{
public:
	CarStateAccele(Car* car);		//コンストラクタ。
	~CarStateAccele();				//デストラクタ。

protected:
	void Enter()override final;		//事前処理。
	void Leave()override final;		//事後処理。
	void Execute()override final;	//実行処理。
private:
	prefab::CSoundSource* m_acceleSound = nullptr;
};

