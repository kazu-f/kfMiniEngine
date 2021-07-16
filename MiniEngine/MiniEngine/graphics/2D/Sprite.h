#pragma once

#include "graphics/GPUBuffer/Indexbuffer.h"
#include "graphics/GPUBuffer/VertexBuffer.h"
#include "graphics/GPUBuffer/ConstantBuffer.h"
#include "graphics/DescriptorHeap.h"

namespace Engine {
	class Texture;

	//スプライトに設定できる最大テクスチャ数。
	const int MAX_TEXTURE = 16;
	//拡張SRVが設定されるレジスタの開始番号。
	const int EXPAND_SRV_REG__START_NO = 10;

	class IShaderResource;

	template< class TExpandData > struct SpriteExpandDataInfo {
		TExpandData* m_expandData = nullptr;
		int m_expandDataSize = 0;
	};
	/// <summary>
	/// スプライトの初期化データ。
	/// </summary>
	struct SpriteInitData {
		const char* m_ddsFilePath[MAX_TEXTURE] = { nullptr };		//DDSファイルのファイルパス。
		Texture* m_textures[MAX_TEXTURE] = { nullptr };			//使用するテクスチャ。DDSファイルのパスが指定されている場合は、このパラメータは無視されます。
		const char* m_vsEntryPointFunc = "VSMain";				//頂点シェーダーのエントリーポイント。
		const char* m_psEntryPoinFunc = "PSMain";				//ピクセルシェーダーのエントリーポイント。
		const char* m_fxFilePath = nullptr;						//.fxファイルのファイルパス。
		UINT m_width = 0;										//スプライトの幅。
		UINT m_height = 0;										//スプライトの高さ。
		bool m_isAlpha = true;									//αブレンディング有効？
		void* m_expandConstantBuffer = nullptr;					//ユーザー拡張の定数バッファ
		int m_expandConstantBufferSize = 0;						//ユーザー拡張の定数バッファのサイズ。
		IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
	};
	/// <summary>
	/// スプライトクラス。
	/// </summary>
	class Sprite {
	public:
		static const Vector2	DEFAULT_PIVOT;					//!<ピボット。
		static const char* SPRITE_SHADER_PATH;					//!<スプライトのシェーダーのファイルパス。
		~Sprite();
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="initData">初期化データ</param>
		void Init(const SpriteInitData& initData, bool isDraw3D = false);
		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		/// <param name="pivot">
		/// ピボット
		/// 0.5, 0.5で画像の中心が基点。
		/// 0.0, 0.0で画像の左下。
		/// 1.0, 1.0で画像の右上。
		/// UnityのuGUIに準拠。
		/// </param>
		void Update(const Vector3& pos, const Quaternion& rot, const Vector3& scale, const Vector2& pivot = DEFAULT_PIVOT);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="viewMat">ビュー行列。</param>
		/// <param name="projMat">プロジェクション行列。</param>
		void Draw(RenderContext& renderContext, const Matrix& viewMat, const Matrix& projMat);
	public:		//Set関数。
		void SetMulColor(const Vector4& col)
		{
			m_mulColor = col;
		}
		/// <summary>
		/// ディスクリプタヒープを初期化。
		/// </summary>
		/// <param name="initData"></param>
		void InitDescriptorHeap(const SpriteInitData& initData);

	private:
		/// <summary>
		/// テクスチャを初期化。
		/// </summary>
		/// <param name="initData"></param>
		void InitTextures(const SpriteInitData& initData);
		/// <summary>
		/// シェーダーを初期化。
		/// </summary>
		/// <param name="initData"></param>
		void InitShader(const SpriteInitData& initData);
		/// <summary>
		/// 頂点バッファとインデックスバッファを初期化。
		/// </summary>
		/// <param name="initData"></param>
		void InitVertexBufferAndIndexBuffer(const SpriteInitData& initData);
		/// <summary>
		/// パイプラインステートを初期化する。
		/// </summary>
		void InitPipelineState(bool isDraw3D, bool isAlpha);
		/// <summary>
		/// 定数バッファを初期化。
		/// </summary>
		/// <param name="initData"></param>
		void InitConstantBuffer(const SpriteInitData& initData);
	private:
		IndexBuffer m_indexBuffer;			//インデックスバッファ。
		VertexBuffer m_vertexBuffer;		//頂点バッファ。
		int m_numTexture = 0;				//テクスチャの枚数。
		Texture m_textures[MAX_TEXTURE];	//テクスチャ。
		Texture* m_textureExternal[MAX_TEXTURE] = { nullptr };	//外部から指定されたテクスチャ
		Vector3 m_position;				//座標。
		Vector2 m_size;						//サイズ。
		Vector4 m_mulColor = Vector4::White;	//乗算カラー。
		Quaternion m_rotation;			//回転。
		Matrix m_world;					//ワールド行列。

		struct LocalConstantBuffer {
			Matrix mvp;
			Vector4 mulColor;
			Vector4 screenParam;
		};
		LocalConstantBuffer m_constantBufferCPU;	//CPU側の定数バッファ。
		ConstantBuffer		m_constantBufferGPU;	//GPU側の定数バッファ。
		ConstantBuffer		m_userExpandConstantBufferGPU;	//ユーザー拡張の定数バッファ(GPU側)
		void* m_userExpandConstantBufferCPU = nullptr;		//ユーザー拡張の定数バッファ(CPU側)
		DescriptorHeap		m_descriptorHeap;		//ディスクリプタヒープ。
		RootSignature		m_rootSignature;		//ルートシグネチャ。
		PipelineState		m_pipelineState;		//パイプラインステート。
		Shader				m_vs;					//頂点シェーダー。
		Shader				m_ps;					//ピクセルシェーダー。
	};
}