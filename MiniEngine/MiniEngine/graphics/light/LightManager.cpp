#include "stdafx.h"
#include "LightManager.h"
#include "prefab/light/DirectionLight.h"

namespace Engine {
	using namespace prefab;

	void CLightManager::Init()
	{
		//アンビエントライトの初期化。
		SetAmbientLight({ 0.5f,0.5f,0.5f });
		//ライトのパラメータ用の定数バッファを作成。
		m_lightParamCB.Init(sizeof(SLightParam), nullptr);
		//ディレクションライト用のストラクチャバッファを作成。
		m_directionLightSB.Init(sizeof(SDirectionLight), MAX_DIRECTION_LIGHT, nullptr);
	}

	void CLightManager::AddLight(prefab::CLightBase* light)
	{

		//ライトの振り分け
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CLightBase)) {

		}
		else if (typeInfo == typeid(CDirectionLight)) {
			//登録済みか調べる。
			auto findIt = std::find(m_directionLidhts.begin(), m_directionLidhts.end(), light);
			if (findIt == m_directionLidhts.end()) {
				//新規登録。
				m_directionLidhts.push_back(reinterpret_cast<CDirectionLight*>(light));
			}
			else {
				//既に登録されている。
				return;
			}
		}
	}

	void CLightManager::RemoveLight(prefab::CLightBase* light)
	{
		//登録を解除する。
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CLightBase)) {
			
		}
		else if(typeInfo == typeid(CDirectionLight)){
			m_directionLidhts.erase(
				std::remove(m_directionLidhts.begin(),m_directionLidhts.end(),light),
				m_directionLidhts.end()			
			);
		}
	}

	void CLightManager::LightUpdate()
	{
		//ディレクションライトのストラクチャーバッファを更新。


	}

	void CLightManager::Render(RenderContext& rc)
	{
		//ライトパラメータのデータを更新。
		m_lightParamCB.CopyToVRAM(&m_lightParam);
		//ディレクションライトのデータを更新。
		m_directionLightSB.Update(&m_rawDirectionLight);
	}

	void CLightManager::SendLightDataToGPU(RenderContext& rc)
	{
		//特に処理なかったわ。
	}

}	////////namespace Engine
