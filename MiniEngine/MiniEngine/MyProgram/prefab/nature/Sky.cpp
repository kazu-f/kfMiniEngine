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
			//モデルレンダー。
			m_modelRender = NewGO<prefab::ModelRender>(0);
			m_modelData.m_tkmFilePath = m_skyModelFilePath.c_str();
			m_modelData.m_shaderData.vsFxFilePath = CPipelineStatesDefault::MODEL_SHADER_PAHT;
			m_modelData.m_shaderData.vsEntryPointFunc = CPipelineStatesDefault::VS_MODEL_ENTRY_POINT;
			m_modelRender->Init(m_modelData);

			m_skyCube.InitFromDDSFile(m_skyCubeMapFilePath.c_str());

			return true;
		}

		void CSky::Update()
		{
		}


	}//namespace prefab;
}//namespace Engine;

