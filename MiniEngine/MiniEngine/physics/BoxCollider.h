#pragma once

#include "physics/ICollider.h"

namespace Engine {

	class CBoxCollider : public ICollider
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CBoxCollider();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CBoxCollider();
		/// <summary>
		/// ボックスコライダーを作成。
		/// </summary>
		void Create(const Vector3& size);
		/// <summary>
		/// シェイプを取得。
		/// </summary>
		btCollisionShape* GetBody()const override
		{
			return shape;
		}
	private:
		btBoxShape* shape = nullptr;		//シェイプ。

	};///class CBoxCollider;

}///namespace Engine;

