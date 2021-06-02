#include "stdafx.h"
#include "DefferdShading.h"

namespace Engine {
	namespace {
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};

		const wchar_t* FX_FILE_PATH = L"Assets/shader/defferd.fx";
		const char* VS_ENTRY = "VSMain";
		const char* PS_ENTRY = "PSMain";

	}

	void CDefferdShading::Init()
	{
		//シェーダー。
		InitShader();
		//パイプラインステート。
		InitPipelineState();
		//定数バッファ。
		InitConstantBuffer();
		//ディスクリプタヒープ。
		CreateDescriptorHeap();
		//四角形プリミティブ。
		InitPrimitive();
	}
	void CDefferdShading::Render(RenderContext& rc)
	{
		//定数バッファの更新。
		SDefferdCB cb;
		cb.mViewProjInv = MainCamera().GetViewProjectionMatrix();
		cb.mViewProjInv.Inverse();			//逆行列にする。
		m_constantBuffer.CopyToVRAM(cb);

		auto& mainRT = GraphicsEngine()->GetMainRenderTarget();

		//メインレンダリングターゲット描画可能待ち。
		rc.WaitUntilToPossibleSetRenderTarget(mainRT);
		//
		rc.SetRenderTargetAndViewport(&mainRT);
		//ルートシグネチャを設定。
		rc.SetRootSignature(CPipelineStatesDefault::m_modelDrawRootSignature);
		//パイプラインステートを設定。
		rc.SetPipelineState(m_defferdPipelineState);
		//ディスクリプタヒープを設定する。
		rc.SetDescriptorHeap(m_defferdDescriptorHeap);
		//フルスクリーン描画。
		m_primitive.Draw(rc);
		//描画完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
	}

	void CDefferdShading::InitPrimitive()
	{
		SSimpleVertex vertices[] =
		{
			{
				Vector4(-1.0f,-1.0f,0.0f,1.0f),
				Vector2(0.0f,1.0f)
			},
			{
				Vector4(1.0f,-1.0f,0.0f,1.0f),
				Vector2(1.0f,1.0f)
			},
			{
				Vector4(-1.0f,1.0f,0.0f,1.0f),
				Vector2(0.0f,0.0f)
			},
			{
				Vector4(1.0f,1.0f,0.0f,1.0f),
				Vector2(1.0f,0.0f)
			}
		};
		short indices[] = { 0,1,2,3 };
		PrimitiveInitData primitiveData;
		//頂点バッファの情報
		primitiveData.vertexSize = sizeof(vertices);
		primitiveData.vertexStride = sizeof(SSimpleVertex);
		primitiveData.pSrcVertexBuffer = vertices;
		//インデックスバッファの情報
		primitiveData.indexSize = sizeof(indices);
		primitiveData.indexStride = sizeof(short);
		primitiveData.pSrcIndexBuffer = indices;
		//プリミティブトポロジー。
		primitiveData.topology = enPrimitiveTopology_TriangleStrip;

		//作成。
		m_primitive.Create(primitiveData);
	}
	void CDefferdShading::InitShader()
	{
		m_vsDefferd.LoadVS(FX_FILE_PATH, VS_ENTRY);
		m_psDefferd.LoadPS(FX_FILE_PATH, PS_ENTRY);
	}
	void CDefferdShading::InitPipelineState()
	{
		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = CPipelineStatesDefault::m_modelDrawRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsDefferd.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psDefferd.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.BlendState.IndependentBlendEnable = FALSE;
		psoDesc.SampleDesc.Count = 1;
		m_defferdPipelineState.Init(psoDesc);
	}
	void CDefferdShading::InitConstantBuffer()
	{
		//定数バッファの作成。
		m_constantBuffer.Init(sizeof(SDefferdCB), nullptr);
	}
	void CDefferdShading::CreateDescriptorHeap()
	{
		//G-Buffer。
		CGBufferRender* gBuffer = GraphicsEngine()->GetGBuffer().get();
		//ライトマネージャー。
		auto* ligMgr = GraphicsEngine()->GetLightManager().get();
		//定数バッファ。
		m_defferdDescriptorHeap.RegistConstantBuffer(0, m_constantBuffer);
		m_defferdDescriptorHeap.RegistConstantBuffer(1, ligMgr->GetLightParamConstantBuffer());
		//シェーダーリソース。
		int srvNo = 0;
		for (; srvNo < static_cast<int>(EnGBuffer::enGBufferNum); srvNo++)
		{
			m_defferdDescriptorHeap.RegistShaderResource(srvNo, gBuffer->GetGBufferTexture(static_cast<EnGBuffer>(srvNo)));
		}
		m_defferdDescriptorHeap.RegistShaderResource(srvNo, m_cubeMapTexture);
		srvNo++;
		m_defferdDescriptorHeap.RegistShaderResource(srvNo, ligMgr->GetDirectionLightStructuredBuffer());
		srvNo++;


		//作成。
		m_defferdDescriptorHeap.Commit();
	}
}