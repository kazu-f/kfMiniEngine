#pragma once

class btCollisionShape;

namespace Engine {

	/// <summary>
	/// コライダーのインターフェースクラス。
	/// </summary>
	class ICollider : Noncopyable
	{
	public:
		virtual ~ICollider() {};
		virtual btCollisionShape* GetBody()const = 0;
	};
}

