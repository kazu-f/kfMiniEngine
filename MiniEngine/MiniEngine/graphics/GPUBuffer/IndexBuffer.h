#pragma once

/// <summary>
/// インデックスバッファ。
/// </summary>
class IndexBuffer {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~IndexBuffer(); 
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="size">インデックスバッファのサイズ。</param>
	/// <param name="stride">ストライド。</param>
	void Init(int size, int stride) ;
	/// <summary>
	/// インデックスデータをインデックスバッファにコピー。
	/// </summary>
	/// <param name="srcIndecies">コピー元のインデックスデータ。</param>
	void Copy(void* srcIndecies) ;
	/// <summary>
	/// インデックスバッファビューを取得。
	/// </summary>
	/// <returns></returns>
	D3D12_INDEX_BUFFER_VIEW& GetView()
	{
		return m_indexBufferView;
	}
	/// <summary>
	/// インデックスの数を取得。
	/// </summary>
	/// <returns>インデックスの数。</returns>
	int GetCount() const
	{
		return m_count;
	}
private:
	ID3D12Resource* m_indexBuffer = nullptr;	//インデックスバッファ。
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;	//インデックスバッファビュー。
	int m_count = 0;							//インデックスの数。
};