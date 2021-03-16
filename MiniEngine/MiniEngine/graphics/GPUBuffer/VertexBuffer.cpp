#include "stdafx.h"
#include "VertexBuffer.h"

namespace Engine {

	VertexBuffer::~VertexBuffer()
	{
		if (m_vertexBuffer) {
			m_vertexBuffer->Release();
		}
	}
	void VertexBuffer::Init(int size, int stride)
	{
		auto d3dDevice = GraphicsEngine()->GetD3DDevice();
		auto d3dxHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto d3dxResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
		d3dDevice->CreateCommittedResource(
			&d3dxHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&d3dxResourceDesc,
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
		auto& rc = GraphicsEngine()->GetRenderContext();
		auto device = GraphicsEngine()->GetD3DDevice();

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

		re.End(GraphicsEngine()->GetCommandQueue());

	}
}