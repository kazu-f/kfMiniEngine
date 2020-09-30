#pragma once


/// <summary>
/// 頂点バッファ
/// </summary>
class VertexBuffer {
		
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~VertexBuffer();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="size">頂点バッファのサイズ</param>
	/// <param name="stride">頂点バッファのストライド。一個の頂点のサイズ</param>
	void Init(int size, int stride);
	/// <summary>
	/// 頂点データを頂点バッファにコピー。
	/// </summary>
	/// <param name="srcVertices">コピー元の頂点データ。</param>
	void Copy(void* srcVertices) ;
	/// <summary>
	/// 頂点バッファビューを取得。
	/// </summary>
	/// <returns>ビュー</returns>
	D3D12_VERTEX_BUFFER_VIEW& GetView()
	{
		return m_vertexBufferView;
	}
private:
	ID3D12Resource* m_vertexBuffer = nullptr;		//頂点バッファ。
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;	//頂点バッファビュー。
};