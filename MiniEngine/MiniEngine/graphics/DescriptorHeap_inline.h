#pragma once
namespace Engine {
	inline ID3D12DescriptorHeap* DescriptorHeap::Get()
	{
		auto backBufferIndex = GraphicsEngine()->GetBackBufferIndex();
		return m_descriptorHeap[backBufferIndex];
	}
	inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetConstantBufferGpuDescritorStartHandle() const
	{
		auto backBufferIndex = GraphicsEngine()->GetBackBufferIndex();
		return m_cbGpuDescriptorStart[backBufferIndex];
	}
	inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetShaderResourceGpuDescritorStartHandle() const
	{
		auto backBufferIndex = GraphicsEngine()->GetBackBufferIndex();
		return m_srGpuDescriptorStart[backBufferIndex];
	}
	inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetUavResourceGpuDescritorStartHandle() const
	{
		auto backBufferIndex = GraphicsEngine()->GetBackBufferIndex();
		return m_uavGpuDescriptorStart[backBufferIndex];
	}
}