#pragma once
#include "MapChipRender.h"
#include "physics/PhysicsStaticObject.h"

namespace Engine {
	struct SLevelObjectData;
	class CMapChipRender;
	/// <summary>
	/// マップチップ。
	/// </summary>
	class CMapChip:Noncopyable
	{
	public:
		CMapChip(const SLevelObjectData& objData,CMapChipRender* mapChipRender);
		~CMapChip();

	private:
		CMapChipRender* m_mapChipRender = nullptr;		//マップチップレンダー。
		CPhysicsStaticObject m_physicsStaticObject;		//静的物理オブジェクト。
	};	//class CMapChip;
}	//namespace Engine;

