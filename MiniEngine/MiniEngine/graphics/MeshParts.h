/// <summary>
/// メッシュパーツクラス。
/// </summary>

#pragma once

#include "tkFile/TkmFile.h"
#include "graphics/GPUBuffer/StructuredBuffer.h"
namespace Engine {
	class RenderContext;
	class Skeleton;
	class IMaterial;
	class IShaderResource;

	/// <summary>
	/// メッシュ
	/// </summary>
	struct SMesh {
		VertexBuffer m_vertexBuffer;						//頂点バッファ。
		std::vector< IndexBuffer* >		m_indexBufferArray;	//インデックスバッファ。
		std::vector< IMaterial* >		m_materials;			//マテリアル。
		std::vector<int>				skinFlags;				//スキンを持っているかどうかのフラグ。
	};

	typedef std::function<void(IMaterial*)>	OnFindMateral;

	/// <summary>
	/// メッシュパーツ。
	/// </summary>
	class MeshParts {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~MeshParts();
		/// <summary>
		/// tkmファイルから初期化
		/// </summary>
		/// <param name="tkmFile">tkmファイル。</param>
		/// /// <param name="fxFilePath">fxファイルのファイルパス</param>
		/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
		/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
		/// <param name="instancingDataSB">インスタンシング描画要のデータのSB</param>
		/// <param name="maxInstance">インスタンスの最大数</param>
		void InitFromTkmFile(
			const TkmFile& tkmFile,
			const SShaderInitData& shaderData,
			void* expandData,
			int expandDataSize,
			IShaderResource* expandShaderResourceView,
			StructuredBuffer* instancingDataSB = nullptr,
			int maxInstance = 1
		);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mWorld">ワールド行列</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		/// <param name="light">ライト</param>
		void Draw(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj);
		/// <summary>
		/// スケルトンを関連付ける。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		void BindSkeleton(Skeleton& skeleton);
		/// <summary>
		/// 自己発光色を設定する。
		/// </summary>
		/// <param name="emission">自己発光色。</param>
		void SetEmissionColor(const Vector3& emission)
		{
			m_emissionColor = emission;
		}
		/// <summary>
		/// シャドウレシーバーかどうか。
		/// </summary>
		void SetShadowReceiverFlag(bool flag)
		{
			m_isShadowReceiver = flag;
		}
		typedef std::function<void(SMesh*)>		OnFindMesh;
		/// <summary>
		/// メッシュの検索。
		/// </summary>
		/// <param name="findMesh">メッシュを見つけた時に呼び出されるコールバック関数。</param>
		void FindMesh(OnFindMesh findMesh) const;
		/// <summary>
		/// モデルマテリアルの検索。
		/// </summary>
		/// <param name="findMaterial">マテリアルを見つけたときに呼び出されるコールバック関数。</param>
		void FindMaterial(OnFindMateral findMaterial) const;

	private:
		/// <summary>
		/// tkmメッシュからメッシュを作成。
		/// </summary>
		/// <param name="mesh">メッシュ</param>
		/// <param name="meshNo">メッシュ番号</param>
		/// <param name="fxFilePath">fxファイルのファイルパス</param>
		/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
		/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
		void CreateMeshFromTkmMesh(
			const TkmFile::SMesh& mesh,
			int meshNo,
			const SShaderInitData& shaderData);

		/// <summary>
		/// ディスクリプタヒープを作成。
		/// </summary>
		void CreateDescriptorHeaps();
	private:
		//拡張SRVが設定されるレジスタの開始番号。
		const int EXPAND_SRV_REG__START_NO = 10;
		/// <summary>
		/// 定数バッファ。
		/// </summary>
		/// <remarks>
		/// この構造体を変更したら、SimpleModel.fxのCBも変更するように。
		/// </remarks>
		struct SConstantBuffer {
			Matrix mWorld;			//ワールド行列。
			Matrix mView;			//ビュー行列。
			Matrix mProj;			//プロジェクション行列。
			Vector3 emissionColor = Vector3::Zero;	//自己発光色。
			int isShadowReceiver = 0;	//シャドウレシーバーフラグ。
		};
		ConstantBuffer m_commonConstantBuffer;					//メッシュ共通の定数バッファ。
		ConstantBuffer m_expandConstantBuffer;					//ユーザー拡張用の定数バッファ
		IShaderResource* m_expandShaderResourceView = nullptr;	//ユーザー拡張シェーダーリソースビュー。
		StructuredBuffer m_boneMatricesStructureBuffer;			//ボーン行列の構造化バッファ。
		std::vector< SMesh* > m_meshs;							//メッシュ。
		std::vector< DescriptorHeap > m_descriptorHeap;			//ディスクリプタヒープ。
		Skeleton* m_skeleton = nullptr;							//スケルトン。
		StructuredBuffer* m_instancingDataPtr = nullptr;		//インスタンシング用描画のデータが格納されたSBのポインタ。
		int m_maxInstance = 1;									//インスタンスの最大数。
		void* m_expandData = nullptr;							//ユーザー拡張データ。
		bool m_isCreateDescriptHeap = false;					//ディスクリプタヒープが作成されているか。
		Vector3 m_emissionColor = Vector3::Zero;				//自己発光色。
		bool m_isShadowReceiver = false;						//シャドウレシーバーフラグ。
	};
}