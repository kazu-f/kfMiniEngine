#pragma once

class btCollisionShape;

namespace Engine {

	/// <summary>
	/// コライダーのインターフェースクラス。
	/// </summary>
	class ICollider
	{
	public:
		virtual btCollisionShape* GetBody()const = 0;
	};
}

