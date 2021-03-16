#include "stdafx.h"
#include "DescriptorHeap.h"

namespace Engine {
	DescriptorHeap::~DescriptorHeap()
	{
		for (auto& ds : m_descriptorHeap) {
			if (ds) {
				ds->Release();
			}
		}
	}

	void DescriptorHeap::Commit()
	{
		const auto& d3dDevice = GraphicsEngine()->GetD3DDevice();
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = m_numShaderResource + m_numConstantBuffer + m_numUavResource;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		for (auto& ds : m_descriptorHeap) {
			auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&ds));
			if (FAILED(hr)) {
				MessageBox(nullptr, "DescriptorHeap::Commit ディスクリプタヒープの作成に失敗しました。", "エラー", MB_OK);
				std::abort();
			}
		}
		//定数バッファやシェーダーリソースのディスクリプタをヒープに書き込んでいく。
		int bufferNo = 0;
		for (auto& descriptorHeap : m_descriptorHeap) {
			auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

			//定数バッファを登録していく。
			for (int i = 0; i < m_numConstantBuffer; i++) {
				if (m_constantBuffers[i] != nullptr) {
					m_constantBuffers[i]->RegistConstantBufferView(cpuHandle, bufferNo);
				}
				//次に進める。
				cpuHandle.ptr += GraphicsEngine()->GetCbrSrvDescriptorSize();
			}

			//続いてシェーダーリソース。
			for (int i = 0; i < m_numShaderResource; i++) {
				if (m_shaderResources[i] != nullptr) {
					m_shaderResources[i]->RegistShaderResourceView(cpuHandle, bufferNo);
				}
				//次に進める。
				cpuHandle.ptr += GraphicsEngine()->GetCbrSrvDescriptorSize();
			}

			//続いてUAV。
			for (int i = 0; i < m_numUavResource; i++) {
				if (m_uavResoruces[i] != nullptr) {
					m_uavResoruces[i]->RegistUnorderAccessView(cpuHandle, bufferNo);
				}
				//次に進める。
				cpuHandle.ptr += GraphicsEngine()->GetCbrSrvDescriptorSize();
			}

			//定数バッファのディスクリプタヒープの開始ハンドルを計算。
			m_cbGpuDescriptorStart[bufferNo] = gpuHandle;
			//シェーダーリソースのディスクリプタヒープの開始ハンドルを計算。
			m_srGpuDescriptorStart[bufferNo] = gpuHandle;
			m_srGpuDescriptorStart[bufferNo].ptr += (UINT64)GraphicsEngine()->GetCbrSrvDescriptorSize() * m_numConstantBuffer;
			//UAVリソースのディスクリプタヒープの開始ハンドルを計算。
			m_uavGpuDescriptorStart[bufferNo] = gpuHandle;
			m_uavGpuDescriptorStart[bufferNo].ptr += (UINT64)GraphicsEngine()->GetCbrSrvDescriptorSize() * (m_numShaderResource + m_numConstantBuffer);
			bufferNo++;
		}
	}

}