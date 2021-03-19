#pragma once

#include "physics/ICollider.h"

namespace Engine {
	/// <summary>
	/// 球体コライダー。
	/// </summary>
	class CSphereCollider : public ICollider
	{
	public:
		//コンストラクタ。
		CSphereCollider();
		//デストラクタ。
		~CSphereCollider();
		/// <summary>
		/// 球体コライダーを作成。
		/// </summary>
		/// <param name="radius">球の半径。</param>
		void Create(const float radius);
		/// <summary>
		/// シェイプを取得。
		/// </summary>
		btCollisionShape* GetBody() const override
		{
			return shape.get();
		}
	private:
		std::unique_ptr<btSphereShape> shape;
	};///class CSphereCollider;
}///namespace Engine;

