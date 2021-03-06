#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
namespace Engine {
	enum ModelUpAxis {
		enModelUpAxis_Z,	//Zアップ
		enModelUpAxis_Y,	//Yアップ。
	};
	class IShaderResource;

	/// <summary>
	/// モデルの初期化データ
	/// </summary>
	struct ModelInitData {
		const char* m_tkmFilePath = nullptr;		//tkmファイルパス。
		SShaderInitData m_shaderData;				//読み込むシェーダーのデータ。頂点シェーダーとピクセルシェーダーの両方を指定する必要がある。
		void* m_expandConstantBuffer = nullptr;		//ユーザー拡張の定数バッファ。
		int m_expandConstantBufferSize = 0;			//ユーザー拡張の定数バッファのサイズ。
		IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。1111
		ModelUpAxis m_modelUpAxis = enModelUpAxis_Z;	//モデルの上方向。
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
		/// インスタンシング描画用のデータ更新の前に呼ぶ。
		/// </summary>
		void ResetInstancingDatas()
		{
			//for (int i = 0; i < m_maxInstance; i++)
			//{
			//	m_instancingData[i] = Matrix::Identity;
			//}
			m_numInstance = 0;
		}
		/// <summary>
		/// インスタンシング描画用の行列データを更新する。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		/// <param name="isCulling">カリングをするかどうか。</param>
		void UpdateInstancingData(
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale,
			bool isCulling
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
		/// インスタンシング描画か？
		/// </summary>
		bool IsInstancing()
		{
			return m_maxInstance > 1;
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
		/// 自己発光色を設定する。
		/// </summary>
		/// <param name="emission">自己発光色。</param>
		void SetEmissionColor(const Vector3& emission)
		{
			m_meshParts.SetEmissionColor(emission);
		}
		/// <summary>
		/// シャドウレシーバーに設定する。
		/// </summary>
		void SetShadowReceiverFlag(bool flag)
		{
			m_meshParts.SetShadowReceiverFlag(flag);
		}
		/// <summary>
		/// カリングするときの遠平面を指定。
		/// </summary>
		void SetCullingFar(const float culfar)
		{
			m_cullingFar = culfar;
		}
		/// <summary>
		/// tkmファイルを取得。
		/// </summary>
		const TkmFile& GetTkmFile()const
		{
			return m_tkmFile;
		}
		/// <summary>
		/// モデルマテリアルの検索。
		/// </summary>
		/// <param name="findMaterial">マテリアルを見つけたときに呼び出されるコールバック関数。</param>
		void FindMaterial(OnFindMateral findMaterial)
		{
			m_meshParts.FindMaterial(findMaterial);
		}
	private:

		Matrix m_world;			//ワールド行列。
		TkmFile m_tkmFile;		//tkmファイル。
		MeshParts m_meshParts;	//メッシュパーツ。
		std::unique_ptr<Matrix[]> m_instancingData;	//インスタンシング描画用のデータ。
		StructuredBuffer m_instancingDataSB;		//インスタンシング描画用のバッファ。
		int m_maxInstance = 1;		//インスタンシング描画の最大数。
		int m_numInstance = 0;		//インスタンスの数。
		float m_cullingFar = 0.0f;
		ModelUpAxis m_modelUpAxis = enModelUpAxis_Z;	//モデルの上方向。
	};
}