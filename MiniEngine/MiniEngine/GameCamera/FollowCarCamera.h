#pragma once

class Car;

/// <summary>
/// 車を追従するカメラ。
/// </summary>
class FollowCarCamera:public IGameObject
{
public:
	FollowCarCamera();					//コンストラクタ。
	~FollowCarCamera();					//デストラクタ。
	bool Start()override final;			//初期化処理。
	void Update()override final;		//更新処理。
	void OnDestroy()override final;		//削除時処理。

public:
	//追従する車を指定する。
	void SetTargetCar(Car* car)
	{
		m_car = car;
	}
	//

private:
	Car* m_car = nullptr;


};

