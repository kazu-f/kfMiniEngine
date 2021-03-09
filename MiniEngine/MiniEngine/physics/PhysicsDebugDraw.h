#pragma once

/// <summary>
/// コリジョンのデバッグ表示
/// </summary>

namespace Engine {

	class CPhysicsDebugDraw : public btIDebugDraw
	{
	public:
		CPhysicsDebugDraw();
		~CPhysicsDebugDraw();
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// 描画の事前処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void BeginDraw(RenderContext& rc)
		{
			m_renderContext = &rc;
			m_numLine = 0;
		}
		/// <summary>
		/// 描画の終了処理。
		/// </summary>
		void EndDraw();
		/// <summary>
		/// 線を描画する。
		/// </summary>
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)override;

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override
		{
		}
		void reportErrorWarning(const char* warningString) override
		{
		}
		void draw3dText(const btVector3& location, const char* textString) override
		{
		}

		void setDebugMode(int debugMode)override
		{
			m_debugMode = debugMode;
		}

		int getDebugMode()const override
		{
			return m_debugMode;
		}
	private:
		//バッファの初期化。
		void InitBuffer();
		//シェーダーの初期化。
		void InitShader();
		//パイプラインステートの初期化。
		void InitPipelineState();
		//ディスクリプタヒープの初期化。
		void InitDescriptorHeap();

	private:
		//定数バッファ。
		struct SConstantBuffer
		{
			Matrix mView;		//ビュー行列。
			Matrix mProj;		//プロジェクション行列。
		};
		int m_debugMode = btIDebugDraw::DBG_NoDebug;
		RenderContext* m_renderContext = nullptr;			//レンダーコンテキスト。
		Shader m_vs;										//頂点シェーダー。
		Shader m_ps;										//ピクセルシェーダー。
		VertexBuffer m_vertexBuffer;						//頂点バッファ。
		IndexBuffer m_indexBuffer;							//インデックスバッファ。
		ConstantBuffer m_cb;								//定数バッファ。
		RootSignature m_rootSignature;						//ルートシグネチャ。
		DescriptorHeap		m_descriptorHeap;				//ディスクリプタヒープ。
		PipelineState m_pipeLineState;						//パイプラインステート。
		static const int VERTEX_MAX = 1000000;
		std::array<Vector4, VERTEX_MAX> m_vertexData;		//頂点データ。
		int m_numLine = 0;									//ライン数。

	};///class CPhysicsDebugDraw;

}///namespace Engine;
