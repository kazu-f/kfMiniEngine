#include "stdafx.h"
#include "VertexBuffer.h"


VertexBuffer::~VertexBuffer()
{
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
	}
}
void VertexBuffer::Init(int size, int stride)
{
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));
	
	m_vertexBuffer->SetName(L"VertexBuffer");
	//頂点バッファのビューを作成。
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = size;
	m_vertexBufferView.StrideInBytes = stride;
}
void VertexBuffer::Copy(void* srcVertices)
{
	auto& rc = g_graphicsEngine->GetRenderContext();
	auto device = g_graphicsEngine->GetD3DDevice();
	
	DirectX::ResourceUploadBatch re(device);
	re.Begin();
	D3D12_SUBRESOURCE_DATA subResourceData;
	subResourceData.pData = srcVertices;
	subResourceData.RowPitch = m_vertexBufferView.SizeInBytes;
	subResourceData.SlicePitch = 1;
	re.Upload(
		m_vertexBuffer,
		0,
		&subResourceData,
		1);

	re.End(g_graphicsEngine->GetCommandQueue());

}