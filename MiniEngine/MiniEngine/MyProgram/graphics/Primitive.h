#pragma once

#include "graphics/GPUBuffer/VertexBuffer.h"
#include "graphics/GPUBuffer/IndexBuffer.h"

namespace Engine {
	/// <summary>
	/// プリミティブトポロジー。
	/// </summary>
	enum EnPrimitiveTopology {
		enPrimitiveTopology_TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		enPrimitiveTopology_TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		enPrimitiveTopology_Undex = 0xFFFFFFFF
	};
	/// <summary>
	/// プリミティブデータ。
	/// </summary>
	struct PrimitiveInitData {
		EnPrimitiveTopology topology;			//トポロジー。
		int vertexSize = 0;						//頂点データのサイズ。
		int vertexStride = 0;					//頂点のサイズ。
		void* pSrcVertexBuffer = nullptr;		//頂点データ。
		int indexSize = 0;						//インデックスバッファデータのサイズ。
		int indexStride = 0;					//インデックスバッファのサイズ。
		void* pSrcIndexBuffer = nullptr;		//インデックスデータ。

	};

	/// <summary>
	/// プリミティブ。
	/// </summary>
	class CPrimitive : Noncopyable
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CPrimitive();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CPrimitive();
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="initData">初期化データ。</param>
		/// <returns>trueで作成成功。</returns>
		void Create(PrimitiveInitData& initData);
		/// <summary>
		/// 頂点バッファを取得。
		/// </summary>
		VertexBuffer& GetVertexBuffer()
		{
			return m_vertexBuffer;
		}
		/// <summary>
		/// インデックスバッファを取得。
		/// </summary>
		IndexBuffer& GetIndexBuffer()
		{
			return m_indexBuffer;
		}
		/// <summary>
		/// プリミティブトポロジーを取得。
		/// </summary>
		D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology()
		{
			return static_cast<D3D12_PRIMITIVE_TOPOLOGY>(m_topology);
		}

	private:
		VertexBuffer m_vertexBuffer;			//頂点データ。
		IndexBuffer m_indexBuffer;				//インデックスバッファ。
		EnPrimitiveTopology m_topology = enPrimitiveTopology_Undex;		 //トポロジー。
	};

}

