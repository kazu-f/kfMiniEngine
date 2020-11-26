#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
namespace Engine {
	class IShaderResource;

	/// <summary>
	/// モデルの初期化データ
	/// </summary>
	struct ModelInitData {
		const char* m_tkmFilePath = nullptr;		//tkmファイルパス。
		const char* m_vsEntryPointFunc = "VSMain";	//頂点シェーダーのエントリーポイント。
		const char* m_psEntryPointFunc = "PSMain";	//ピクセルシェーダーのエントリーポイント。
		const char* m_fxFilePath = nullptr;			//.fxファイルのファイルパス。
		void* m_expandConstantBuffer = nullptr;		//ユーザー拡張の定数バッファ。
		int m_expandConstantBufferSize = 0;			//ユーザー拡張の定数バッファのサイズ。
		IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
	};
	/// <summary>
	/// モデルクラス。
	/// </summary>
	class Model {

	public:

		/// <summary>
		/// tkmファイルから初期化。
		/// </summary>
		/// <param name="initData">初期化データ</param>
		void Init(const ModelInitData& initData,int maxInstance = 1);
		/// <summary>
		/// ワールド行列の更新。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);
		/// <summary>
		/// インスタンシング描画用の行列データを更新する。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		void UpdateInstancingData(
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		);
		/// <summary>
		/// GPUにインスタンシング描画用のデータを送る。
		/// </summary>
		void SendGPUInstancingDatas()
		{
			if (m_maxInstance > 1) {
				m_instancingDataSB.Update(m_instancingData.get());
			}
		}

		/// <summary>
		/// スケルトンを関連付ける。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		void BindSkeleton(Skeleton& skeleton)
		{
			m_meshParts.BindSkeleton(skeleton);
		}

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		void Draw(RenderContext& renderContext);
		/// <summary>
		/// /シャドウマップ用描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mLVP">ライトビュープロジェクション行列</param>
		void Draw(RenderContext& rc, Matrix mLVP);
		/// <summary>
		/// 初期化済みか？
		/// </summary>
		bool IsInited()
		{
			return m_tkmFile.IsLoaded();
		}
		/// <summary>
		/// ワールド行列を取得。
		/// </summary>
		/// <returns></returns>
		const Matrix& GetWorldMatrix() const
		{
			return m_world;
		}
		/// <summary>
		/// シャドウレシーバーに設定する。
		/// </summary>
		void SetShadowReceiverFlag(bool flag)
		{
			m_meshParts.SetShadowReceiverFlag(flag);
		}

	private:

		Matrix m_world;			//ワールド行列。
		TkmFile m_tkmFile;		//tkmファイル。
		MeshParts m_meshParts;	//メッシュパーツ。
		std::unique_ptr<Matrix[]> m_instancingData;	//インスタンシング描画用のデータ。
		StructuredBuffer m_instancingDataSB;		//インスタンシング描画用のバッファ。
		int m_maxInstance = 1;		//インスタンシング描画の最大数。
		int m_numInstance = 0;		//インスタンスの数。
	};
}