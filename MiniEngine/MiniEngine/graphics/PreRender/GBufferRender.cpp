#include "stdafx.h"
#include "GBufferRender.h"

namespace Engine {
	CGBufferRender::CGBufferRender()
	{
	}
	CGBufferRender::~CGBufferRender()
	{
	}
	void CGBufferRender::Init()
	{
		//�A���x�h�o�b�t�@�̏������B
		auto& albedo = GetRenderTarget(EnGBuffer::enGBufferAlbed);
		albedo.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT
		);
		//�@���o�b�t�@�̏������B
		auto& normal = GetRenderTarget(EnGBuffer::enGBufferNormal);
		normal.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferWidth(),
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		//�ڃx�N�g���o�b�t�@�̏������B
		auto& tangent = GetRenderTarget(EnGBuffer::enGBufferTangent);
		tangent.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		//�X�y�L�����o�b�t�@�̏������B
		auto& spec = GetRenderTarget(EnGBuffer::enGBufferSpecular);
		spec.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		//�e�}�b�v�̏������B
		auto& shadow = GetRenderTarget(EnGBuffer::enGBufferShadow);
		shadow.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);


	}
	void CGBufferRender::BeginRender(RenderContext& rc)
	{
		const int arraySize = ARRAYSIZE(m_GBuffer);
		RenderTarget* rts[arraySize] = { nullptr };
		//�����_�����O�^�[�Q�b�g��ݒ�B
		for (int i = 0; i < arraySize; i++) {
			rts[i] = &m_GBuffer[i];
		}
		//�����_�����O�^�[�Q�b�g�ݒ�҂��B
		rc.WaitUntilToPossibleSetRenderTargets(arraySize, rts);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rc.SetRenderTargets(arraySize, rts);
		//�����_�����O�^�[�Q�b�g���N���A�B
		rc.ClearRenderTargetViews(arraySize, rts);
	}
	void CGBufferRender::EndRender(RenderContext& rc)
	{
		g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
	}
}