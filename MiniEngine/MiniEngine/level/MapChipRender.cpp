#include "stdafx.h"
#include "MapChipRender.h"

namespace Engine {
	CMapChipRender::CMapChipRender()
	{
	}
	CMapChipRender::~CMapChipRender()
	{
	}
	void CMapChipRender::OnDestroy()
	{
		DeleteGO(m_modelRender);
	}
	void CMapChipRender::Update()
	{
	}
	void CMapChipRender::InitAfterAddAllRenderObjects()
	{
		//���f�������_�[�̏������B
		m_modelRender = NewGO<prefab::ModelRender>(0);
		//�t�@�C���p�X�쐬�B
		wchar_t filePath[256];
		swprintf_s(filePath, L"Assets/modelData/%s.tkm", m_renderObjDatas[0].name);		//�����t�@�C���p�X�̊J�n�n�_������ӂ�B

		size_t oriSize = wcslen(filePath) + 1;
		size_t convertedChars = 0;
		char strConcat[] = "";
		size_t strConcatSize = (strlen(strConcat) + 1) * 2;
		const size_t newSize = oriSize * 2;
		char* nString = new char[newSize + strConcatSize];			//�����͉���Y��ɂȂ�Ȃ����H
		wcstombs_s(&convertedChars, nString, newSize, filePath, _TRUNCATE);
		_mbscat_s((unsigned char*)nString, newSize + strConcatSize, (unsigned char*)strConcat);

		ModelInitData initData;
		initData.m_fxFilePath = "Assets/shader/model.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_tkmFilePath = nString;
		m_modelRender->Init(initData);
		m_modelRender->SetPosition(m_renderObjDatas[0].position);
		m_modelRender->SetRotation(m_renderObjDatas[0].rotation);
		m_modelRender->SetScale(m_renderObjDatas[0].scale);
	}
}