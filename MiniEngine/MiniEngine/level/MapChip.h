#pragma once
#include "MapChipRender.h"

namespace Engine {
	struct SLevelObjectData;
	class CMapChipRender;
	/// <summary>
	/// �}�b�v�`�b�v�B
	/// </summary>
	class CMapChip:Noncopyable
	{
	public:
		CMapChip(const SLevelObjectData& objData,CMapChipRender* mapChipRender);
		~CMapChip();

	private:
		CMapChipRender* m_mapChipRender = nullptr;		//�}�b�v�`�b�v�����_�[�B

	};	//class CMapChip;
}	//namespace Engine;
