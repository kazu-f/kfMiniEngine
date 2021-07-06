#pragma once

namespace Engine {
	/// <summary>
	/// よく使うパイプラインステートをまとめる。
	/// </summary>
	class CPipelineStatesDefault
	{
	public://シェーダーのファイルパス等。
		static const wchar_t* MODEL_SHADER_PATH;						//モデルシェーダーのファイルパス。
		static const char* VS_MODEL_ENTRY_POINT;						//通常モデルの頂点シェーダー。
		static const char* VS_MODELINSTANCING_ENTRY_POINT;				//インスタンシングモデルの頂点シェーダー。
		static const char* VS_SKINMODEL_ENTRY_POINT;					//スキンモデルの頂点シェーダー。
		static const char* VS_SKINMODELINTANCING_ENTRY_POINT;			//スキンありインスタンシングモデルの頂点シェーダー。
		static const char* VS_MODEL_SHADOW_ENTRY_POINT;					//シャドウマップ用頂点シェーダー。
		static const char* VS_MODEL_SHADOWINSTANCING_ENTRY_POINT;		//シャドウマップ・インスタンシングモデル用頂点シェーダー。
		static const char* VS_SKINMODEL_SHADOW_ENTRY_POINT;				//シャドウマップ・スキンありモデル用頂点シェーダー。
		static const char* VS_SKINMODEL_SHADOWINSTANCING_ENTRY_POINT;	//シャドウマップ・スキンありインスタンシングモデル用頂点シェーダー。
		static const char* PS_GBUFFER_ENTRY_POINT;						//G-Bfferに書き込むピクセルシェーダー。
		static const char* PS_SHADOW_ENTRY_POINT;						//シャドウマップに書き込むピクセルシェーダー。
		static const char* PS_TRANS_ENTRY_POINT;						//フォワード用頂点シェーダー。

		static const wchar_t* COPY_SHADER_PATH;							//コピー用シェーダーのファイルパス。
		static const char* VS_COPY_ENTRY_POINT;							//コピー用頂点シェーダー。
		static const char* PS_COPY_ENTRY_POINT;							//コピー用ピクセルシェーダー。

	public://モデル用の設定。
		static RootSignature m_modelDrawRootSignature;			//モデル描画用のルートシグネチャ。
		static Shader m_vsModel;								//モデル用の頂点シェーダー。
		static Shader m_vsModelInstancing;						//インスタンシングモデル用の頂点シェーダー。
		static Shader m_vsSkinModel;							//モデル用の頂点シェーダー。(スキンあり)
		static Shader m_vsSkinModelInstancing;					//インスタンシングモデル用の頂点シェーダー。(スキンあり)
		static Shader m_psModel;								//モデル用のピクセルシェーダー。(G-Buffer書き込み。)
		static Shader m_vsModelShadowMap;						//シャドウマップのモデル用の頂点シェーダー。
		static Shader m_vsModelShadowInstancing;				//シャドウマップのインスタンスモデル用。
		static Shader m_vsSkinModelShadowMap;					//シャドウマップのモデル用の頂点シェーダー。(スキンあり)
		static Shader m_vsSkinModelShadowInstancing;			//シャドウマップのインスタンスモデル用。(スキンあり)
		static Shader m_psModelShadowMap;						//シャドウマップのモデル用のピクセルシェーダー。
		static Shader m_psTransModel;							//半透明のモデル用のピクセルシェーダー。

		static PipelineState m_ModelPipelineState;				//モデル用のパイプラインステート。
		static PipelineState m_ModelInstancingPipelineState;	//インスタンシングモデル用のパイプラインステート。
		static PipelineState m_SkinModelPipelineState;			//モデル用のパイプラインステート。(スキンあり)
		static PipelineState m_SkinModelInstancingPipelineState;//インスタンシングモデル用のパイプラインステート。(スキンあり)
		static PipelineState m_transModelPipelineState;			//モデル用のパイプラインステート(半透明マテリアル)。
		static PipelineState m_transSkinModelPipelineState;			//モデル用のパイプラインステート(半透明マテリアル)。(スキンあり)
		static PipelineState m_transModelInstancingPipelineState;			//インスタンシングモデル用のパイプラインステート(半透明マテリアル)。
		static PipelineState m_transSkinModelInstancingPipelineState;			//インスタンシングモデル用のパイプラインステート(半透明マテリアル)。(スキンあり)
		static PipelineState m_ModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。
		static PipelineState m_ModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。
		static PipelineState m_SkinModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。(スキンあり)
		static PipelineState m_SkinModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。(スキンあり)

	public:	//コピー用。
		static Shader m_vsCopy;									//コピー用の頂点シェーダー。
		static Shader m_psCopy;									//コピー用のピクセルシェーダー。

		static RootSignature m_copyRootSignature;				//コピー用のルートシグネチャ。
		static PipelineState m_copyPipelineState;				//コピー用パイプラインステート。

	public:
		/// <summary>
		/// 初期化を行う。
		/// </summary>
		static void Init();
	private:
		//シェーダーの初期化。
		static void InitShaders();
		//ルートシグネチャの初期化。
		static void InitRootSignature();
		//モデル用パイプラインステート初期化。
		static void InitModelPipelineState();
		//コピー用パイプラインステート初期化。
		static void InitCopyPipelineState();

	};///class CPipelineStatesDefault;

}///namespace Engine;
