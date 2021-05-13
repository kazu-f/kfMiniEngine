#pragma once

#include "tkFile/TkmFile.h"

namespace Engine {
	/// <summary>
	/// マテリアル。
	/// </summary>
	class IMaterial {
	public:
		/// <summary>
		/// tkmファイルのマテリアル情報から初期化する。
		/// </summary>
		/// <param name="tkmMat">tkmマテリアル</param>
		void InitFromTkmMaterila(
			const TkmFile::SMaterial& tkmMat,
			const wchar_t* fxFilePath,
			const char* vsEntryPointFunc,
			const char* psEntryPointFunc);
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		virtual void BeginRender(RenderContext& rc, int maxInstance = 1);

		/// <summary>
		/// アルベドマップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetAlbedoMap()
		{
			return m_albedoMap;
		}
		/// <summary>
		/// 法線マップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetNormalMap()
		{
			return m_normalMap;
		}
		/// <summary>
		/// スペキュラマップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetSpecularMap()
		{
			return m_specularMap;
		}
		/// <summary>
		/// 定数バッファを取得。
		/// </summary>
		/// <returns></returns>
		ConstantBuffer& GetConstantBuffer()
		{
			return m_constantBuffer;
		}
	protected:
		/// <summary>
		/// パイプラインステートの初期化。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		/// <param name="fxFilePath">fxファイルのファイルパス</param>
		/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
		/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
		virtual void InitShaders(const wchar_t* fxFilePath) = 0;
		/// <summary>
		/// テクスチャを初期化。
		/// </summary>
		/// <param name="tkmMat"></param>
		void InitTexture(const TkmFile::SMaterial& tkmMat);

	protected:
		/// <summary>
		/// マテリアルパラメータ。
		/// </summary>
		struct SMaterialParam {
			int hasNormalMap;	//法線マップを保持しているかどうかのフラグ。
			int hasSpecMap;		//スペキュラマップを保持しているかどうかのフラグ。
		};
		Texture	m_albedoMap;	//アルベドマップ。
		Texture	m_normalMap;							//法線マップ。
		Texture	m_specularMap;							//スペキュラマップ。
		ConstantBuffer m_constantBuffer;				//定数バッファ。
		//RootSignature m_rootSignature;					//ルートシグネチャ。
		//PipelineState m_ModelPipelineState;				//モデル用のパイプラインステート。
		//PipelineState m_ModelInstancingPipelineState;	//インスタンシングモデル用のパイプラインステート。
		//PipelineState m_transModelPipelineState;		//モデル用のパイプラインステート(半透明マテリアル)。
		//PipelineState m_ModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。
		//PipelineState m_ModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。

		//Shader m_vsModel;								//モデル用の頂点シェーダー。
		//Shader m_vsModelInstancing;						//インスタンシングモデル用の頂点シェーダー。
		//Shader m_psModel;								//モデル用のピクセルシェーダー。
		//Shader m_vsModelShadowMap;						//シャドウマップのモデル用の頂点シェーダー。
		//Shader m_vsModelShadowInstancing;				//シャドウマップのインスタンスモデル用。
		//Shader m_psModelShadowMap;						//シャドウマップのモデル用のピクセルシェーダー。
		//Shader m_psTransModel;							//半透明のモデル用のピクセルシェーダー。
	};
	/// <summary>
	/// スキン無しマテリアル。
	/// </summary>
	class NonSkinMaterial : public IMaterial {
	private:
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		/// <param name="fxFilePath">fxファイルのファイルパス</param>
		void InitShaders(const wchar_t* fxFilePath)override final
		{};
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;

	};
	/// <summary>
	/// スキンありマテリアル。
	/// </summary>
	class SkinMaterial :public IMaterial{
	private:
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		/// <param name="fxFilePath">fxファイルのファイルパス</param>
		void InitShaders(const wchar_t* fxFilePath)override final
		{};
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;
	};
}