#pragma once
/// <summary>
/// ライトマネージャー。
/// </summary>

#include "LightData.h"

namespace Engine {
	namespace prefab {
		class CLightBase;
		class CDirectionLight;
		class CPointLight;
	}

	/// <summary>
	/// ライトの管理者。
	/// </summary>
	/// <remarks>
	/// インスタンス化されたLightBaseのインスタンスはすべて登録される。
	/// このクラスはGraphicsEngineクラスのみが保持する。
	/// </remarks>
	class CLightManager : Noncopyable
	{
	public:
		/// <summary>
		/// 初期化処理。
		/// </summary>
		void Init();
		/// <summary>
		/// ライトを追加する。
		/// </summary>
		void AddLight(prefab::CLightBase* light);
		/// <summary>
		/// ライトを削除する。
		/// </summary>
		void RemoveLight(prefab::CLightBase* light);
		/// <summary>
		/// 更新処理。
		/// </summary>
		void LightUpdate();

	public:		//描画関係の処理。
		void Render(RenderContext& rc);

		void SendLightDataToGPU(RenderContext& rc);

	public:	//Get関数,Set関数。
		/// <summary>
		/// ポイントライトの数を取得。
		/// </summary>
		int GetNumPointLight()const
		{
			return static_cast<int>(0);			//後でリストのサイズを渡しておく。
		}
		/// <summary>
		/// アンビエントライトを取得。
		/// </summary>
		const Vector3& GetAmbientLight() const
		{
			return m_lightParam.ambientLight;
		}
		/// <summary>
		/// アンビエントライトを設定。
		/// </summary>
		void SetAmbientLight(const Vector3& ambient)
		{
			m_lightParam.ambientLight = ambient;
		}
		/// <summary>
		/// 共通ライトパラメータの定数バッファを取得。
		/// </summary>
		ConstantBuffer& GetLightParamConstantBuffer()
		{
			return m_lightParamCB;
		}
		/// <summary>
		/// ディレクションライトのストラクチャーバッファを取得。
		/// </summary>
		StructuredBuffer& GetDirectionLightStructuredBuffer()
		{
			return m_directionLightSB;
		}
	private:
		static const int MAX_DIRECTION_LIGHT = 8;				//ディレクションライトの最大数。
		static const int MAX_POINT_LIGHT = 1024;				//ポイントライトの最大数。
		/// <summary>
		/// モデルシェーダーで使用するライト用のパラメータ。
		/// </summary>
		/// <remarks>
		/// メンバ変数を追加したり、並びを変えたりしたら
		/// シェーダー側も変更する必要がある。
		/// </remarks>
		struct SLightParam {
			Vector3 eyePos;			//視線の位置。
			int numDirectionLight;	//ディレクションライトの数。
			Vector3 ambientLight;	//アンビエントライト。(環境光)
			int numPointLight;		//ポイントライトの数。
			Vector4 screenParam;	//スクリーンパラメータ。
		};
		/// <summary>
		/// ライトカリング用のカメラデータ。
		/// </summary>
		struct SLightCullingCameraData {
			Matrix mProj;		//プロジェクション行列。
			Matrix mProjInv;	//プロジェクション行列の逆行列。
			Matrix mCameraRot;	//カメラの回転行列。
		};

		SLightParam	m_lightParam;			//ライトのパラメータ。
		SLightCullingCameraData m_lightCullingCameraData;	//ライトカリング用のカメラデータ。
		SDirectionLight m_rawDirectionLights[MAX_DIRECTION_LIGHT];		//ディレクションライトのデータの配列。
		SPointLight		m_rawPointLights[MAX_POINT_LIGHT];				//ポイントライトのデータの配列。
		std::list<prefab::CDirectionLight*> m_directionLights;			//ディレクションライトの配列。
		std::list<prefab::CPointLight*> m_pointLights;			//ポイントライトの配列。
		Shader m_csLightCulling;				//ライトカリング用のコンピュートシェーダー。
		ConstantBuffer m_lightParamCB;			//ライト情報の定数バッファ。
		ConstantBuffer m_lightCullingCameraCB;	//ライトカリング用のカメラデータの定数バッファ。
		StructuredBuffer m_directionLightSB;	//ディレクションライトのストラクチャバッファ。
		StructuredBuffer m_pointLightSB;		//ポイントライトのストラクチャバッファ。
		RWStructuredBuffer m_pointLightNoListInTileUAV;		//タイルごとのポイントライトの番号のリストを出力するUAV。
	};

}
