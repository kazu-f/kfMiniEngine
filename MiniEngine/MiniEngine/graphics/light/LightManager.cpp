#include "stdafx.h"
#include "LightManager.h"

namespace Engine {

	void CLightManager::Init()
	{
		//アンビエントライトの初期化。

		//ライトのパラメータ用の定数バッファを作成。
		m_lightParamCB.Init(sizeof(SLightParam), nullptr);
		//ディレクションライト用のストラクチャバッファを作成。
		m_directionLightSB.Init(sizeof(SDirectionLight), MAX_DIRECTION_LIGHT, nullptr);
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
