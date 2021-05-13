#pragma once
#include "graphics/Material.h"

namespace Engine {
	/// <summary>
	/// ユーザー指定のシェーダーのマテリアル。
	/// </summary>
	class UserMaterial : public IMaterial {
	public:
		/// <summary>
		/// tkmファイルのマテリアル情報から初期化する。
		/// </summary>
		/// <param name="tkmMat">tkmマテリアル</param>
		void InitFromTkmMaterila(
			const TkmFile::SMaterial& tkmMat,
			const SShaderInitData& shaderData) override final;
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;
	private:
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		/// <param name="fxFilePath">fxファイルのファイルパス</param>
		/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
		/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
		void InitShaders(const SShaderInitData& shaderData);
		/// <summary>
		/// パイプラインステートの初期化。
		/// </summary>
		void InitPipelineState();

	private:
		RootSignature m_rootSignature;					//ルートシグネチャ。
		PipelineState m_ModelPipelineState;				//モデル用のパイプラインステート。
		PipelineState m_transModelPipelineState;		//モデル用のパイプラインステート(半透明マテリアル)。

		Shader m_vsModel;								//モデル用の頂点シェーダー。
		Shader m_psModel;								//モデル用のピクセルシェーダー。
		Shader m_psTransModel;							//半透明のモデル用のピクセルシェーダー。
	};///class UserMaterial;

}///namespace Engine;

