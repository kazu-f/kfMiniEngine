#include "stdafx.h"
#include "ShadowMap.h"

namespace Engine {
	void ShadowMap::Init(SShadowMapConfig& config)
	{
		m_isEnable = config.isEnable;
		if (!m_isEnable) return;		//無効だったら戻る。

		//シャドウマップの解像度の設定。
		int wh[NUM_SHADOW_MAP][2] = {
			{config.shadowMapWidth,config.lightHeight},					//近距離
			{config.shadowMapWidth >> 1,config.shadowMapHeight >> 1},	//中距離。
			{config.shadowMapWidth >> 1,config.shadowMapHeight >> 1}	//遠距離。
		};
		//クリアカラーを作成。
		float clearColor[] = {
			1.0f,1.0f,1.0f,1.0f
		};
		//シャドウマップ用のレンダーターゲット作成。
		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP;shadowMapNo++) {
			m_shadowMaps[shadowMapNo].Create(
				wh[shadowMapNo][0],		//幅
				wh[shadowMapNo][1],		//高さ
				1,						//
				1,						//
				DXGI_FORMAT_R32_FLOAT,	//カラーフォーマット。
				DXGI_FORMAT_R32_FLOAT,	//デプスステンシルフォーマット。
				clearColor				//クリアカラー。
			);
		}

		//定数バッファを初期化する。
		m_shadowCb.Init(sizeof(m_shadowCbEntity), nullptr);
	}
	void ShadowMap::RenderToShadowMap(RenderContext& rc)
	{
	}
	void ShadowMap::WaitEndRenderToShadowMap(RenderContext& rc)
	{
	}
	void ShadowMap::Update()
	{
	}
	Vector3 ShadowMap::CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition)
	{
		return Vector3();
	}
}
