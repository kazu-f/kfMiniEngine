#pragma once
/// <summary>
/// ライトマネージャー。
/// </summary>

#include "LightData.h"

namespace Engine {
	namespace prefab {
		class CLightBase;
		class CDirectionLight;
	}

	/// <summary>
	/// ライトの管理者。
	/// </summary>
	/// <remarks>
	/// インスタンス化されたLightBaseのインスタンスはすべて登録される。
	/// このクラスはGraphicsEngineクラスのみが保持する。
	/// </remarks>
	class CLightManager
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
		SLightParam	m_lightParam;			//ライトのパラメータ。
		SDirectionLight m_rawDirectionLight[MAX_DIRECTION_LIGHT];		//ディレクションライトのデータの配列。
		SPointLight		m_rawPointLights[MAX_POINT_LIGHT];				//ポイントライトのデータの配列。
		/// <summary>
		/// ここにディレクションライトクラスとポイントライトクラスのリスト。
		/// </summary>
		std::list<prefab::CDirectionLight*> m_directionLidhts;			//ディレクションライトの配列。
		ConstantBuffer m_lightParamCB;		//定数バッファ。
		StructuredBuffer m_directionLightSB;//ディレクションライトのストラクチャバッファ。
	};

}
