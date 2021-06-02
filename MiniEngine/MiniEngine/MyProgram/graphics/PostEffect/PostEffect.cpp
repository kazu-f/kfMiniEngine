#include "stdafx.h"
#include "PostEffect.h"

namespace Engine {
	namespace {
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
	}
	CPostEffect::CPostEffect()
	{
	}
	CPostEffect::~CPostEffect()
	{
	}
	void CPostEffect::Release()
	{
	}
	void CPostEffect::Create(const SPostEffectConfig& config)
	{
		//トーンマップを初期化。
		m_tonemap.Init(config);
		//ブルームを初期化。
		m_bloom.Init(config);
		//アンチエイリアスを初期化。
		m_fxaa.Init(config);
		//四角形プリミティブを初期化。
		InitPrimitive();
	}
	void CPostEffect::Render(RenderContext& rc)
	{
		//トーンマップ。
		m_tonemap.Render(rc,this);
		//ブルーム。
		m_bloom.Render(rc,this);
		//アンチ。
		m_fxaa.Render(rc,this);
	}
	void CPostEffect::DrawFullScreenQuad(RenderContext& rc)
	{
		m_quadPrimitive.Draw(rc);
	}
	void CPostEffect::InitPrimitive()
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
		m_quadPrimitive.Create(primitiveData);
	}
}
