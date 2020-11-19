#include "stdafx.h"
#include "Level.h"
#include "MapChip.h"
#include "MapChipRender.h"

namespace Engine {
	CLevel::~CLevel()
	{
	}
	void CLevel::CreateMapChipRenderOrAddRenderObject(const SLevelObjectData& objData)
	{
	}
	void CLevel::BuildBoneMatrices()
	{
	}
	void CLevel::Init(const char* filePath, std::function<bool(SLevelObjectData& objData)> hookFunc)
	{
	}
}