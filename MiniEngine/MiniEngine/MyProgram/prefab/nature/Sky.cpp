#include "stdafx.h"
#include "Sky.h"
#include "graphics/Material.h"

namespace Engine {
	namespace prefab {

		namespace {
			const wchar_t* SKY_SHADER_FILEPATH = nullptr;
			const char* PS_SKY_ENTRY_POINT = nullptr;
		}


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
			m_modelData.m_shaderData.psFxFilePath = SKY_SHADER_FILEPATH;
			m_modelData.m_shaderData.psEntryPointFunc = PS_SKY_ENTRY_POINT;
			m_modelRender->Init(m_modelData);

			m_skyCube.InitFromDDSFile(m_skyCubeMapFilePath.c_str());
			m_modelRender->FindMaterial([&](auto* mat) {
				mat->SetAlbedoMap(m_skyCube);
				});

			//初回は更新が必要。
			m_isDirty = true;

			return true;
		}

		void CSky::Update()
		{
			//更新の必要がある。
			if (m_isDirty) {
				m_modelRender->SetPosition(m_position);
				m_modelRender->SetScale(m_scale);

				m_isDirty = false;
			}
		}


	}//namespace prefab;
}//namespace Engine;

