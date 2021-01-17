#pragma once

#include "physics/ICollider.h"

namespace Engine {

	class CCapsuleCollider : public ICollider
	{
	public:
		/// <summary>
		/// カプセルコライダーの作成。
		/// </summary>
		/// <param name="radius">半径。</param>
		/// <param name="height">高さ。</param>
		void Create(float radius,float height) 
		{
			m_shape = std::make_unique<btCapsuleShape>(radius, height);
			m_radius = radius;
			m_height = height;
		}
		//シェイプの取得。
		btCollisionShape* GetBody()const override
		{
			return m_shape.get();
		}
		//半径の取得。
		float GetRadius() const
		{
			return m_radius;
		}
		//高さの取得。
		float GetHeight()const
		{
			return m_height;
		}

	private:
		std::unique_ptr<btCapsuleShape> m_shape;	//カプセルシェイプ。
		float m_radius = 0.0f;		//半径。
		float m_height = 0.0f;		//高さ。
	};///CCapsuleCollider
}///namespace Engine
