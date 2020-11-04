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
		//アルベドバッファの初期化。
		auto& albedo = GetRenderTarget(EnGBuffer::enGBufferAlbed);
		albedo.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT
		);
		//法線バッファの初期化。
		auto& normal = GetRenderTarget(EnGBuffer::enGBufferNormal);
		normal.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferWidth(),
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		//接ベクトルバッファの初期化。
		auto& tangent = GetRenderTarget(EnGBuffer::enGBufferTangent);
		tangent.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		//スペキュラバッファの初期化。
		auto& spec = GetRenderTarget(EnGBuffer::enGBufferSpecular);
		spec.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		//影マップの初期化。
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
		//レンダリングターゲットを設定。
		for (int i = 0; i < arraySize; i++) {
			rts[i] = &m_GBuffer[i];
		}
		//レンダリングターゲット設定待ち。
		rc.WaitUntilToPossibleSetRenderTargets(arraySize, rts);
		//レンダリングターゲットを設定。
		rc.SetRenderTargets(arraySize, rts);
		//レンダリングターゲットをクリア。
		rc.ClearRenderTargetViews(arraySize, rts);
	}
	void CGBufferRender::EndRender(RenderContext& rc)
	{
		g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
	}
}