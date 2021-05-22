#pragma once
/*
*	FXAA
*/

namespace Engine {
	class PostEffect;
	/// <summary>
	/// FXAA
	/// ポストエフェクトによるアンチエイリアスを行う。
	/// </summary>
	class CFxaa : Noncopyable
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">コンフィグ</param>
		void Init(const SPostEffectConfig& config);
		
		void Render(RenderContext& rc);
	private:
		/// <summary>
		/// シェーダーの初期化。
		/// </summary>
		void InitShader();
		/// <summary>
		/// パイプラインステートの作成。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// ディスクリプタヒープの作成。
		/// </summary>
		void CreateDescriptorHeap();
	private:
		Shader m_vsShader;
		Shader m_psShader;
		RootSignature m_rootSignature;
		PipelineState m_fxaaPipelineState;
		DescriptorHeap m_descriptorHeap;
		bool m_isEnable = false;
	};//class Fxaa;

}///namespace Engine;

