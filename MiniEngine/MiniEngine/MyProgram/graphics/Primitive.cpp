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
	m_vertexBuffer.Init(initData.numVertex, initData.vertexStride);
	m_indexBuffer.Init(initData.numIndex, initData.indexStride);
	m_vertexBuffer.Copy(initData.pSrcVertexBuffer);
	m_indexBuffer.Copy(initData.pSrcIndexBuffer);
	m_topology = initData.topology;
}
