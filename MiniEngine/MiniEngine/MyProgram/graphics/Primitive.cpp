#include "stdafx.h"
#include "Primitive.h"

Engine::CPrimitive::CPrimitive()
{
}

Engine::CPrimitive::~CPrimitive()
{
}

void Engine::CPrimitive::Create(PrimitiveInitData& initData)
{
	m_vertexBuffer.Init(initData.vertexSize, initData.vertexStride);
	m_indexBuffer.Init(initData.indexSize, initData.indexStride);
	m_vertexBuffer.Copy(initData.pSrcVertexBuffer);
	m_indexBuffer.Copy(initData.pSrcIndexBuffer);
	m_topology = initData.topology;
}

void Engine::CPrimitive::Draw(RenderContext& rc)
{
	//頂点バッファを設定。
	rc.SetVertexBuffer(m_vertexBuffer);
	//インデックスバッファを設定。
	rc.SetIndexBuffer(m_indexBuffer);
	//プリミティブトポロジーを設定。
	rc.SetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(m_topology));
	//描画。
	rc.DrawIndexed(m_indexBuffer.GetCount(), 1);
}
