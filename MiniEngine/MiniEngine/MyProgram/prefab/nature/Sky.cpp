#include "stdafx.h"
#include "Sky.h"

namespace Engine {
	namespace prefab {



		CSky::CSky()
		{
		}

		CSky::~CSky()
		{
		}

		void CSky::OnDestroy()
		{
			DeleteGO(m_modelRender);
		}

		bool CSky::Start()
		{
			////���f�������_�[�B
			//m_modelRender = NewGO<prefab::ModelRender>(0);
			//m_modelData.m_tkmFilePath = m_skyModelFilePath.c_str();

			//m_modelRender->Init(m_modelData);

			//if (m_modelRender->IsStart())
			//{
			//	m_skyCube.InitFromDDSFile(m_skyCubeMapFilePath.c_str());

			//}

			return true;
		}

		void CSky::Update()
		{
		}


	}//namespace prefab;
}//namespace Engine;
