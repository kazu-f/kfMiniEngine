#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"

namespace Engine {
	CMapChip::CMapChip(const SLevelObjectData& objData, CMapChipRender* mapChipRender)
	{
		//char objName[256];
		//wcstombs(objName, objData.name, 256);
		////ファイルパスを作成。
		//wchar_t filePath[256];
		//swprintf_s(filePath, L"Assets/modelData/%s.tkm", objData.name);
		m_mapChipRender = mapChipRender;
		m_physicsStaticObject.CreateMesh(
			objData.position,
			objData.rotation,
			objData.scale,
			mapChipRender->GetModelRender()
		);
	}
	CMapChip::~CMapChip()
	{

	}
}