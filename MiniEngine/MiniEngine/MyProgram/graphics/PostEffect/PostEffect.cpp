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

		//四角形プリミティブを初期化。
		InitPrimitive();
	}
	void CPostEffect::Render(RenderContext& rc)
	{
		rc.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		rc.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		rc.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());
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
		//頂点の数を記録しておく。
		m_primitiveVertexNum = primitiveData.vertexSize / primitiveData.indexStride;
	}
}
