#include "stdafx.h"
#include "DefferdShading.h"

namespace Engine {
	void CDefferdShading::Init(CGBufferRender* gBuffer)
	{
		//�傫���ݒ�B
		m_initData.m_width = FRAME_BUFFER_W;
		m_initData.m_height = FRAME_BUFFER_H;
		//�t�@�C���p�X�B
		m_initData.m_fxFilePath = "Assets/shader/defferd.fx";
		m_initData.m_vsEntryPointFunc = "VSMain";
		m_initData.m_psEntryPoinFunc = "PSMain";
		//�e�N�X�`���̐ݒ�B
		int i = 0;
		for (; i < static_cast<int>(EnGBuffer::enGBufferNum); i++)
		{
			m_initData.m_textures[i] = &gBuffer->GetGBufferTexture(static_cast<EnGBuffer>(i));
		}
		m_initData.m_textures[i] = &m_cubeMapTexture;

		m_defferd.Init(m_initData);

		m_defferd.Update(Vector3::Zero, Quaternion::Identity, Vector3::One);
	}
	void CDefferdShading::Render(RenderContext& rc)
	{
		m_defferd.Update(Vector3::Zero, Quaternion::Identity, Vector3::One);
		m_defferd.Draw(rc, MainCamera2D().GetViewMatrix(), MainCamera2D().GetProjectionMatrix());
	}
}