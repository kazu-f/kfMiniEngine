#pragma once

namespace Engine {
	/// <summary>
	/// よく使うパイプラインステートをまとめる。
	/// </summary>
	class CPipelineStatesDefault
	{
	public:
		static void Init();
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
		static PipelineState m_ModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。
		static PipelineState m_ModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。
		static PipelineState m_SkinModelShadowPipelineState;		//シャドウマップのモデル用のパイプラインステート。(スキンあり)
		static PipelineState m_SkinModelShadowInstancingPipelineState;		//シャドウマップのインスタンスモデル用。(スキンあり)

	private:
		//シェーダーの初期化。
		static void InitShaders();
		//ルートシグネチャの初期化。
		static void InitRootSignature();
		//パイプラインステート初期化。
		static void InitPipelineState();

	};///class CPipelineStatesDefault;

}///namespace Engine;
