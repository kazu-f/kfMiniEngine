#pragma once

namespace Engine {
	/// <summary>
	/// ディスクリプタヒープ。
	/// </summary>
	class DescriptorHeap {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~DescriptorHeap();
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		ID3D12DescriptorHeap* Get();
		/// <summary>
		/// シェーダーリソースをディスクリプタヒープに登録。
		/// </summary>
		/// <param name="registerNo">レジスタ番号</param>
		/// <param name="sr">シェーダーリソース</param>
		void RegistShaderResource(int registerNo, IShaderResource& sr)
		{
			if (registerNo < MAX_SHADER_RESOURCE) {
				m_shaderResources[registerNo] = &sr;
				if (m_numShaderResource < registerNo + 1) {
					m_numShaderResource = registerNo + 1;
				}
			}
			else {
				MessageBox(nullptr, L"DescriptorHeap::RegistShaderResource() レジスタ番号が範囲外です。", L"エラー", MB_OK);
				std::abort();
			}
		}
		/// <summary>
		/// アンオーダーアクセスリソースを登録。
		/// </summary>
		/// <param name="registerNo">レジスタ番号</param>
		/// <param name="sr">アンオーダーリソース</param>
		void RegistUnorderAccessResource(int registerNo, IUnorderAccessResrouce& sr)
		{
			if (registerNo < MAX_SHADER_RESOURCE) {
				m_uavResoruces[registerNo] = &sr;
				if (m_numUavResource < registerNo + 1) {
					m_numUavResource = registerNo + 1;
				}
			}
			else {
				MessageBox(nullptr, L"DescriptorHeap::RegistShaderResource() レジスタ番号が範囲外です。", L"エラー", MB_OK);
				std::abort();
			}
		}
		/// <summary>
		/// 定数バッファをディスクリプタヒープに登録。
		/// </summary>
		/// <param name="registerNo">レジスタ番号</param>
		/// <param name="cb">定数バッファ</param>
		void RegistConstantBuffer(int registerNo, ConstantBuffer& cb)
		{
			if (registerNo < MAX_CONSTANT_BUFFER) {
				m_constantBuffers[registerNo] = &cb;
				if (m_numConstantBuffer < registerNo + 1) {
					m_numConstantBuffer = registerNo + 1;
				}
			}
			else {
				MessageBox(nullptr, L"DescriptorHeap::RegistConstantBuffer() レジスタ番号が範囲外です。", L"エラー", MB_OK);
				std::abort();
			}
		}
		/// <summary>
		/// ディスクリプタヒープへの登録を確定。
		/// </summary>
		void Commit();
		/// <summary>
		/// 定数バッファのディスクリプタの開始ハンドルを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGpuDescritorStartHandle() const;
		/// <summary>
		/// シェーダーリソースのディスクリプタの開始ハンドルを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGpuDescritorStartHandle() const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetUavResourceGpuDescritorStartHandle() const;
		/// <summary>
		/// シェーダーリソースが一つでも登録されているか判定。
		/// </summary>
		/// <returns></returns>
		bool IsRegistShaderResource() const
		{
			return m_numShaderResource != 0;
		}
		bool IsRegistConstantBuffer() const
		{
			return m_numConstantBuffer != 0;
		}
		bool IsRegistUavResource() const
		{
			return m_numUavResource != 0;
		}
	private:
		enum {
			MAX_SHADER_RESOURCE = 128,	//シェーダーリソースの最大数。
			MAX_CONSTANT_BUFFER = 64,	//定数バッファの最大数。
		};
		int m_numShaderResource = 0;	//シェーダーリソースの数。
		int m_numConstantBuffer = 0;	//定数バッファの数。
		int m_numUavResource = 0;
		ID3D12DescriptorHeap* m_descriptorHeap[2] = { nullptr };					//ディスクリプタヒープ。
		IShaderResource* m_shaderResources[MAX_SHADER_RESOURCE] = { nullptr };		//シェーダーリソース。7
		IUnorderAccessResrouce* m_uavResoruces[MAX_SHADER_RESOURCE] = { nullptr };	//UAVリソース。
		ConstantBuffer* m_constantBuffers[MAX_CONSTANT_BUFFER] = { nullptr };	//定数バッファ。
		D3D12_GPU_DESCRIPTOR_HANDLE m_cbGpuDescriptorStart[2];					//定数バッファのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_srGpuDescriptorStart[2];					//シェーダーリソースのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_uavGpuDescriptorStart[2];					//UAVリソースのディスクリプタヒープの開始ハンドル。
	};
}