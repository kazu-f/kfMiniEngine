#pragma once
#include "MapChipRender.h"

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

	};	//class CMapChip;
}	//namespace Engine;

