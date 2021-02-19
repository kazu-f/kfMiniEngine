#pragma once


#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>

#include "DirectXTK/Inc/DDSTextureLoader.h"
#include "DirectXTK/Inc/ResourceUploadBatch.h"

#include "RenderContext.h"

#include "camera/Camera.h"

namespace Engine {
	class CGameObjectManager;
	class CLightManager;
	class CShadowMap;
	class CGBufferRender;
	class CDefferdShading;

	/// <summary>
	/// DirectX12に依存するグラフィックスエンジン
	/// </summary>
	class GraphicsEngine {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~GraphicsEngine();
		/// <summary>
		/// 初期化
		/// </summary>
		/// <remarks>
		/// 本関数を呼び出すことでDirectX12の初期化が行われます。
		/// </remarks>
		/// <param name="hwnd">Windowハンドル</param>
		/// <param name="frameBufferwidth">フレームバッファの幅</param>
		/// <param name="frameBufferHeight">フレームバッファの高さ</param>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// 事前レンダリングの更新処理。
		/// </summary>
		void PreRenderUpdate();
		/// <summary>
		/// 描画を行う。
		/// </summary>
		void Render(CGameObjectManager* goMgr);
		/// <summary>
		/// シャドウマップ描画前に呼ぶ処理
		/// </summary>
		void BeginRenderShadowMap()
		{
			BeginRender();
		}
		/// <summary>
		/// シャドウマップ描画後に呼ぶ処理
		/// </summary>
		void ExecuteCommand();
	private:
		/// <summary>
		/// プレレンダリング。
		/// </summary>
		void PreRender(CGameObjectManager* goMgr);
		/// <summary>
		/// 描画時に呼ばれる処理。
		/// </summary>
		/// <param name="goMgr"></param>
		void OnRender(CGameObjectManager* goMgr);
		/// <summary>
		/// レンダリング開始。
		/// </summary>
		/// <remarks>
		/// 1フレームのレンダリングの開始時に呼び出してください。
		/// </remarks>
		void BeginRender();
		/// <summary>
		/// レンダリング終了。
		/// </summary>
		/// <remarks>
		/// 1フレームのレンダリングの終了時に呼び出してください。
		/// </remarks>
		void EndRender();
	public:
		/// <summary>
		/// D3Dデバイスを取得。
		/// </summary>
		/// <returns></returns>
		ID3D12Device* GetD3DDevice()
		{
			return m_d3dDevice;
		}
		/// <summary>
		/// バックバッファの番号を取得。
		/// </summary>
		/// <returns>バックバッファの番号。</returns>
		UINT GetBackBufferIndex() const
		{
			return m_frameIndex;
		}
		/// <summary>
		/// コマンドキューを取得。
		/// </summary>
		/// <returns></returns>
		ID3D12CommandQueue* GetCommandQueue() const
		{
			return m_commandQueue;
		}

		/// <summary>
		/// CBR_SRVのディスクリプタのサイズを取得。
		/// </summary>
		/// <returns></returns>
		UINT GetCbrSrvDescriptorSize() const
		{
			return m_cbrSrvDescriptorSize;
		}
		/// <summary>
		/// レンダリングコンテキストを取得。
		/// </summary>
		/// <returns></returns>
		RenderContext& GetRenderContext()
		{
			return m_renderContext;
		}
		/// <summary>
		/// フレームバッファの幅を取得。
		/// </summary>
		/// <returns>フレームバッファの幅。</returns>
		UINT GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/// <summary>
		/// フレームバッファの高さを取得。
		/// </summary>
		/// <returns>フレームバッファの高さ。</returns>
		UINT GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/// <summary>
		/// レンダリングターゲットをフレームバッファに変更する。
		/// </summary>
		/// <param name="rc"></param>
		void ChangeRenderTargetToFrameBuffer(RenderContext& rc);
		/// <summary>
		/// 現在のフレームバッファのレンダリングターゲットビューを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerRTV() const
		{
			return m_currentFrameBufferRTVHandle;
		}
		/// <summary>
		/// ライトマネージャーの取得。
		/// </summary>
		std::unique_ptr<CLightManager>& GetLightManager()
		{
			return m_lightManager;
		}
		/// <summary>
		/// シャドウマップクラスの取得。
		/// </summary>
		std::unique_ptr<CShadowMap>& GetShadowMap()
		{
			return m_shadowMap;
		}
	private:
		/// <summary>
		/// D3Dデバイスの作成。
		/// </summary>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateD3DDevice(IDXGIFactory4* dxgiFactory);
		/// <summary>
		/// コマンドキューの作成。
		/// </summary>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateCommandQueue();
		/// <summary>
		/// スワップチェインの作成
		/// </summary>
		/// <param name="hwnd">Windowハンドル</param>
		/// <param name="frameBufferWidth">フレームバッファの幅</param>
		/// <param name="frameBufferHeight">フレームバッファの高さ</param>
		/// <param name="dxgiFactory">DirectX グラフィックス インフラストラクチャー</param>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateSwapChain(
			HWND hwnd,
			UINT frameBufferWidth,
			UINT frameBufferHeight,
			IDXGIFactory4* dxgiFactory
		);
		/// <summary>
		/// DirectX グラフィックス インフラストラクチャーの作成。
		/// </summary>
		/// <remarks>
		/// DirectX グラフィックス インフラストラクチャーは
		/// カーネル モード ドライバーおよびシステム ハードウェアと通信するためのインターフェースです。 
		/// </remarks>
		/// <returns>作成されたDXGI</returns>
		IDXGIFactory4* CreateDXGIFactory();
		/// <summary>
		/// フレームバッファ用のディスクリプタヒープを作成。
		/// </summary>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateDescriptorHeapForFrameBuffer();
		/// <summary>
		/// フレームバッファ用のレンダリングターゲットビューを作成。
		/// </summary>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateRTVForFameBuffer();
		/// <summary>
		/// フレームバッファ用の深度ステンシルビューを作成。
		/// </summary>
		/// <param name="frameBufferWidth">フレームバッファの幅</param>
		/// <param name="frameBufferHeight">フレームバッファの高さ</param>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateDSVForFrameBuffer(UINT frameBufferWidth, UINT frameBufferHeight);
		/// <summary>
		/// コマンドリストの作成。
		/// </summary>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateCommandList();
		/// <summary>
		/// GPUとの同期オブジェクト作成
		/// </summary>
		/// <returns>trueが返ってきたら作成に成功。</returns>
		bool CreateSynchronizationWithGPUObject();
		/// <summary>
		/// 描画の完了待ち。
		/// </summary>
		void WaitDraw();
	public:
		enum { FRAME_BUFFER_COUNT = 2 };						//フレームバッファの数。
	private:
		//GPUベンダー定義。
		enum GPU_Vender {
			GPU_VenderNvidia,	//NVIDIA
			GPU_VenderAMD,		//Intel
			GPU_VenderIntel,	//AMD
			Num_GPUVender,
		};

		ID3D12Device* m_d3dDevice = nullptr;					//D3Dデバイス。
		ID3D12CommandQueue* m_commandQueue = nullptr;			//コマンドキュー。
		IDXGISwapChain3* m_swapChain = nullptr;					//スワップチェイン。
		ID3D12DescriptorHeap* m_rtvHeap = nullptr;				//レンダリングターゲットビューのディスクリプタヒープ。
		ID3D12DescriptorHeap* m_dsvHeap = nullptr;				//深度ステンシルビューのディスクリプタヒープ。
		ID3D12CommandAllocator* m_commandAllocator = nullptr;	//コマンドアロケータ。
		ID3D12GraphicsCommandList* m_commandList = nullptr;		//コマンドリスト。
		ID3D12PipelineState* m_pipelineState = nullptr;			//パイプラインステート。
		int m_currentBackBufferIndex = 0;						//現在のバックバッファの番号。
		UINT m_rtvDescriptorSize = 0;							//フレームバッファのディスクリプタのサイズ。
		UINT m_dsvDescriptorSize = 0;							//深度ステンシルバッファのディスクリプタのサイズ。
		UINT m_cbrSrvDescriptorSize = 0;						//CBR_SRVのディスクリプタのサイズ。
		ID3D12Resource* m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };	//フレームバッファ用のレンダリングターゲット。
		ID3D12Resource* m_depthStencilBuffer = nullptr;	//深度ステンシルバッファ。
		D3D12_VIEWPORT m_viewport;			//ビューポート。
		D3D12_RECT m_scissorRect;			//シザリング矩形。
		RenderContext m_renderContext;		//レンダリングコンテキスト。
		D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;		//現在書き込み中のフレームバッファのレンダリングターゲットビューのハンドル。
		D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;		//現在書き込み中のフレームバッファの深度ステンシルビューの
		// GPUとの同期で使用する変数。
		UINT m_frameIndex = 0;
		HANDLE m_fenceEvent = nullptr;
		ID3D12Fence* m_fence = nullptr;
		UINT64 m_fenceValue = 0;
		UINT m_frameBufferWidth = 0;		//フレームバッファの幅。
		UINT m_frameBufferHeight = 0;		//フレームバッファの高さ。
		Camera m_camera2D;					//2Dカメラ。
		Camera m_camera3D;					//3Dカメラ。
		std::unique_ptr<CLightManager> m_lightManager;		//ライトマネージャー。
		std::unique_ptr<CShadowMap> m_shadowMap;			//指向性シャドウマップ。
		std::unique_ptr<CGBufferRender> m_gBuffer;			//GBuffer。
		std::unique_ptr<CDefferdShading> m_defferd;			//ディファード。
		int m_vsyncInterval = 1;							//VSyncの間隔
		////エフェクト関係
		//EffekseerRenderer::RendererRef m_renderer;
	};
	extern GraphicsEngine* g_graphicsEngine;	//グラフィックスエンジン
	extern Camera* g_camera2D;					//2Dカメラ。
	extern Camera* g_camera3D;					//3Dカメラ。
}
