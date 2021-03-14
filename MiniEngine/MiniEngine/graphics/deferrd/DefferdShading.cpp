#include "stdafx.h"
#include "DefferdShading.h"

namespace Engine {
	void CDefferdShading::Init(CGBufferRender* gBuffer)
	{
		//大きさ設定。
		m_initData.m_width = FRAME_BUFFER_W;
		m_initData.m_height = FRAME_BUFFER_H;
		//ファイルパス。
		m_initData.m_fxFilePath = "Assets/shader/defferd.fx";
		m_initData.m_vsEntryPointFunc = "VSMain";
		m_initData.m_psEntryPoinFunc = "PSMain";
		//テクスチャの設定。
		for (int i = 0; i < static_cast<int>(EnGBuffer::enGBufferNum); i++)
		{
			m_initData.m_textures[i] = &gBuffer->GetGBufferTexture(static_cast<EnGBuffer>(i));
		}
		
		m_defferd.Init(m_initData);

		m_defferd.Update(Vector3::Zero, Quaternion::Identity, Vector3::One);
	}
	void CDefferdShading::Render(RenderContext& rc)
	{
		m_defferd.Update(Vector3::Zero, Quaternion::Identity, Vector3::One);
		m_defferd.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix());
	}
}