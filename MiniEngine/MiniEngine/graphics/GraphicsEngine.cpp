#include "stdafx.h"
#include "GraphicsEngine.h"

namespace Engine {
	CGraphicsEngine::~CGraphicsEngine()
	{
		WaitDraw();
		//後始末。
		if (m_commandQueue) {
			m_commandQueue->Release();
		}
		if (m_swapChain) {
			m_swapChain->Release();
		}
		if (m_rtvHeap) {
			m_rtvHeap->Release();
		}
		if (m_dsvHeap) {
			m_dsvHeap->Release();
		}
		if (m_commandAllocator) {
			m_commandAllocator->Release();
		}
		if (m_commandList) {
			m_commandList->Release();
		}
		if (m_pipelineState) {
			m_pipelineState->Release();
		}
		for (auto& rt : m_renderTargets) {
			if (rt) {
				rt->Release();
			}
		}
		if (m_depthStencilBuffer) {
			m_depthStencilBuffer->Release();
		}
		if (m_fence) {
			m_fence->Release();
		}

		if (m_d3dDevice) {
			m_d3dDevice->Release();
		}

		CloseHandle(m_fenceEvent);
	}
	void CGraphicsEngine::WaitDraw()
	{
		//描画終了待ち
		// Signal and increment the fence value.
		const UINT64 fence = m_fenceValue;
		m_commandQueue->Signal(m_fence, fence);
		m_fenceValue++;

		// Wait until the previous frame is finished.
		if (m_fence->GetCompletedValue() < fence)
		{
			m_fence->SetEventOnCompletion(fence, m_fenceEvent);
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}
	}
	bool CGraphicsEngine::Init(HWND hwnd, const SInitParam& initParam)
	{
		auto hdc = GetDC(hwnd);
		auto rate = GetDeviceCaps(hdc, VREFRESH);
		if (rate >= 120) {
			m_vsyncInterval = 2;
		}
		m_frameBufferWidth = initParam.frameBuffer_W;
		m_frameBufferHeight = initParam.frameBuffer_H;

		//デバイスにアクセスするためのインターフェースを作成。
		auto dxgiFactory = CreateDXGIFactory();
		//D3Dデバイスの作成。
		if (!CreateD3DDevice(dxgiFactory)) {
			//D3Dデバイスの作成に失敗した。
			MessageBox(hwnd, TEXT("D3Dデバイスの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}
		//コマンドキューの作成。
		if (!CreateCommandQueue()) {
			//コマンドキューの作成に失敗した。
			MessageBox(hwnd, TEXT("コマンドキューの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}
		//スワップチェインを作成。
		if (!CreateSwapChain(hwnd, m_frameBufferWidth, m_frameBufferHeight, dxgiFactory)) {
			//スワップチェインの作成に失敗。
			MessageBox(hwnd, TEXT("スワップチェインの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}

		//フレームバッファ用のディスクリプタヒープを作成する。
		if (!CreateDescriptorHeapForFrameBuffer()) {
			MessageBox(hwnd, TEXT("フレームバッファ用のディスクリプタヒープの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}

		//フレームバッファ用のRTVを作成する。
		if (!CreateRTVForFameBuffer()) {
			MessageBox(hwnd, TEXT("フレームバッファ用のRTVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;

		}

		//フレームバッファ用のDSVを作成する。
		if (!CreateDSVForFrameBuffer(m_frameBufferWidth, m_frameBufferHeight)) {
			MessageBox(hwnd, TEXT("フレームバッファ用のDSVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}

		//コマンドアロケータの作成。
		m_d3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocator));

		if (!m_commandAllocator) {
			MessageBox(hwnd, TEXT("コマンドアロケータの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}

		//コマンドリストの作成。
		if (!CreateCommandList()) {
			MessageBox(hwnd, TEXT("コマンドリストの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}

		//GPUと同期をとるためのオブジェクトを作成する。
		if (!CreateSynchronizationWithGPUObject()) {
			MessageBox(hwnd, TEXT("GPUと同期をとるためのオブジェクトの作成に失敗しました。"), TEXT("エラー"), MB_OK);
			return false;
		}

		//レンダリングコンテキストの作成。
		m_renderContext.Init(m_commandList);

		//メインレンダリングターゲットの初期化。
		float clearCol[] = { 0.5f,0.5f,0.5f,1.0f };
		if(m_mainRenderTarget.Create(
			static_cast<int>(m_frameBufferWidth),
			static_cast<int>(m_frameBufferHeight),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearCol) == false ) {
			ENGINE_ASSERT(false, "メインレンダリングターゲットの作成に失敗。");
			return false;
		}

		//パイプラインステートを初期化。
		CPipelineStatesDefault::Init();

		//ビューポートを初期化。
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = static_cast<FLOAT>(m_frameBufferWidth);
		m_viewport.Height = static_cast<FLOAT>(m_frameBufferHeight);
		m_viewport.MinDepth = D3D12_MIN_DEPTH;
		m_viewport.MaxDepth = D3D12_MAX_DEPTH;

		//シザリング矩形を初期化。
		m_scissorRect.left = 0;
		m_scissorRect.top = 0;
		m_scissorRect.right = m_frameBufferWidth;
		m_scissorRect.bottom = m_frameBufferHeight;

		//CBR_SVRのディスクリプタのサイズを取得。
		m_cbrSrvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


		//初期化が終わったのでDXGIを破棄。
		dxgiFactory->Release();

		//カメラを初期化する。
		m_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_camera2D.SetWidth(static_cast<float>(m_frameBufferWidth));
		m_camera2D.SetHeight(static_cast<float>(m_frameBufferHeight));
		m_camera2D.SetPosition({ 0.0f, 0.0f, -1.0f });
		m_camera2D.SetTarget({ 0.0f, 0.0f, 0.0f });

		m_camera3D.SetPosition({ 0.0f, 50.0f, 200.0f });
		m_camera3D.SetTarget({ 0.0f, 50.0f, 0.0f });


		//ライトマネージャーの作成。
		m_lightManager = std::make_unique<CLightManager>();
		m_lightManager->Init();
		//シャドウマップの作成。
		m_shadowMap = std::make_unique<CShadowMap>();
		m_shadowMap->Init(initParam.graphicsConf.shadowConf);
		//GBufferの作成。
		m_gBuffer = std::make_unique<CGBufferRender>();
		m_gBuffer->Init();
		//ディファード用クラス作成。
		m_defferd = std::make_unique<CDefferdShading>();
		m_defferd->Init();
		//ポストエフェクト。
		m_postEffect = std::make_unique<CPostEffect>();
		m_postEffect->Create(initParam.graphicsConf.postEffectConf);
		//フェード。
		m_fade = std::make_unique<CFade>();
		m_fade->Init();

		//フルスクリーンコピー用のスプライトの初期化。
		SpriteInitData spriteData;
		spriteData.m_fxFilePath = Sprite::SPRITE_SHADER_PATH;
		spriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteData.m_width = static_cast<float>(m_frameBufferWidth);
		spriteData.m_height = static_cast<float>(m_frameBufferHeight);
		spriteData.m_isAlpha = false;			//αブレンディングは無効。
		m_copyFullScreenSprite.Init(spriteData);

		//DXGI_FORMAT effectFormat[] =
		//{
		//	DXGI_FORMAT_R8G8B8A8_UNORM
		//};

		//m_renderer = EffekseerRendererDX12::Create(
		//	GetD3DDevice(),
		//	m_commandQueue,
		//	static_cast<int32_t>(2),
		//	effectFormat,
		//	static_cast<int32_t>(1),
		//	DXGI_FORMAT_D32_FLOAT,
		//	true,
		//	static_cast < int32_t>(2000)
		//);

		return true;
	}

	IDXGIFactory4* CGraphicsEngine::CreateDXGIFactory()
	{
		UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
		//デバッグコントローラーがあれば、デバッグレイヤーがあるDXGIを作成する。
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			debugController->Release();
		}
#endif
		IDXGIFactory4* factory;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
		return factory;
	}

	bool CGraphicsEngine::CreateD3DDevice(IDXGIFactory4* dxgiFactory)
	{
		D3D_FEATURE_LEVEL fuatureLevel[] = {
			D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1の機能を使う。
			D3D_FEATURE_LEVEL_12_0	//Direct3D 12.0の機能を使う。
		};

		IDXGIAdapter* adapterTmp = nullptr;
		IDXGIAdapter* adapterVender[Num_GPUVender] = { nullptr };	//各ベンダーのアダプター。
		IDXGIAdapter* adapterMaxVideoMemory = nullptr;				//最大ビデオメモリのアダプタ。
		IDXGIAdapter* useAdapter = nullptr;							//最終的に使用するアダプタ。
		SIZE_T videoMemorySize = 0;
		for (int i = 0; dxgiFactory->EnumAdapters(i, &adapterTmp) != DXGI_ERROR_NOT_FOUND; i++) {
			DXGI_ADAPTER_DESC desc;
			adapterTmp->GetDesc(&desc);

			if (desc.DedicatedVideoMemory > videoMemorySize) {
				//こちらのビデオメモリの方が多いので、こちらを使う。
				adapterMaxVideoMemory = adapterTmp;
				videoMemorySize = desc.DedicatedVideoMemory;
			}
			if (wcsstr(desc.Description, L"NVIDIA") != nullptr) {
				//NVIDIA製
				adapterVender[GPU_VenderNvidia] = adapterTmp;
			}
			else if (wcsstr(desc.Description, L"AMD") != nullptr) {
				//AMD製
				adapterVender[GPU_VenderAMD] = adapterTmp;
			}
			else if (wcsstr(desc.Description, L"Intel") != nullptr) {
				//Intel製
				adapterVender[GPU_VenderIntel] = adapterTmp;
			}
		}
		//使用するアダプターを決める。
		if (adapterVender[GPU_VenderNvidia] != nullptr) {
			//NVIDIA製が最優先
			useAdapter = adapterVender[GPU_VenderNvidia];
		}
		else if (adapterVender[GPU_VenderAMD] != nullptr) {
			//次はAMDが優先。
			useAdapter = adapterVender[GPU_VenderAMD];
		}
		else {
			//NVIDIAとAMDのGPUがなければビデオメモリが一番多いやつを使う。
			useAdapter = adapterMaxVideoMemory;
		}
		for (auto fuatureLevel : fuatureLevel) {

			auto hr = D3D12CreateDevice(
				useAdapter,
				fuatureLevel,
				IID_PPV_ARGS(&m_d3dDevice)
			);
			if (SUCCEEDED(hr)) {
				//D3Dデバイスの作成に成功した。
				break;
			}
		}
		return m_d3dDevice != nullptr;
	}
	bool CGraphicsEngine::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		auto hr = m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
		if (FAILED(hr)) {
			//コマンドキューの作成に失敗した。
			return false;
		}
		return true;
	}
	bool CGraphicsEngine::CreateSwapChain(
		HWND hwnd,
		UINT frameBufferWidth,
		UINT frameBufferHeight,
		IDXGIFactory4* dxgiFactory
	)
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = FRAME_BUFFER_COUNT;
		swapChainDesc.Width = frameBufferWidth;
		swapChainDesc.Height = frameBufferHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		IDXGISwapChain1* swapChain;
		dxgiFactory->CreateSwapChainForHwnd(
			m_commandQueue,
			hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		);
		//IDXGISwapChain3のインターフェースを取得。
		swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
		swapChain->Release();
		//IDXGISwapChain3のインターフェースを取得。
		m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
		return true;
	}
	bool CGraphicsEngine::CreateDescriptorHeapForFrameBuffer()
	{
		//RTV用のディスクリプタヒープを作成する。
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = FRAME_BUFFER_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		auto hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
		if (FAILED(hr)) {
			//RTV用のディスクリプタヒープの作成に失敗した。
			return false;
		}
		//ディスクリプタのサイズを取得。
		m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//DSV用のディスクリプタヒープを作成する。
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
		if (FAILED(hr)) {
			//DSV用のディスクリプタヒープの作成に失敗した。
			return false;
		}
		//ディスクリプタのサイズを取得。
		m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		return true;
	}
	bool CGraphicsEngine::CreateRTVForFameBuffer()
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

		//フロントバッファをバックバッファ用のRTVを作成。
		for (UINT n = 0; n < FRAME_BUFFER_COUNT; n++) {
			m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			m_d3dDevice->CreateRenderTargetView(
				m_renderTargets[n], nullptr, rtvHandle
			);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}
		return true;
	}
	bool CGraphicsEngine::CreateDSVForFrameBuffer(UINT frameBufferWidth, UINT frameBufferHeight)
	{
		D3D12_CLEAR_VALUE dsvClearValue;
		dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		dsvClearValue.DepthStencil.Depth = 1.0f;
		dsvClearValue.DepthStencil.Stencil = 0;

		CD3DX12_RESOURCE_DESC desc(
			D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			frameBufferWidth,
			frameBufferHeight,
			1,
			1,
			DXGI_FORMAT_D32_FLOAT,
			1,
			0,
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

		auto d3dxHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto hr = m_d3dDevice->CreateCommittedResource(
			&d3dxHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&dsvClearValue,
			IID_PPV_ARGS(&m_depthStencilBuffer)
		);
		if (FAILED(hr)) {
			//深度ステンシルバッファの作成に失敗。
			return false;
		}
		//ディスクリプタを作成
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

		m_d3dDevice->CreateDepthStencilView(
			m_depthStencilBuffer, nullptr, dsvHandle
		);

		return true;
	}
	bool CGraphicsEngine::CreateCommandList()
	{
		//コマンドリストの作成。
		m_d3dDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)
		);
		if (!m_commandList) {
			return false;
		}
		//コマンドリストは開かれている状態で作成されるので、いったん閉じる。
		m_commandList->Close();

		return true;
	}
	bool CGraphicsEngine::CreateSynchronizationWithGPUObject()
	{
		m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		if (!m_fence) {
			//フェンスの作成に失敗した。
			return false;
		}
		m_fenceValue = 1;
		//同期を行うときのイベントハンドラを作成する。
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr) {
			return false;
		}
		return true;
	}
	void CGraphicsEngine::Update()
	{
		//カメラを更新する。
		m_camera2D.Update();
		m_camera3D.Update();

	}
	void CGraphicsEngine::PreRenderUpdate()
	{
		//ライトの更新処理。
		m_lightManager->LightUpdate();
		//シャドウマップの更新処理。
		m_shadowMap->Update();
	}
	void CGraphicsEngine::Render(CGameObjectManager* goMgr)
	{
		BeginRender();

		PreRender(goMgr);
		
		DefferdShading(goMgr);

		ForwardRender(goMgr);

		PostRender(goMgr);

		m_fade->Update();
		m_fade->FadeRender(m_renderContext);

		EndRender();
 	}
	void CGraphicsEngine::PreRender(CGameObjectManager* goMgr)
	{
		//シャドウマップより前に行う処理。
		goMgr->PreRender(m_renderContext);

		//指向性シャドウ回りの処理。
		m_shadowMap->RenderToShadowMap(m_renderContext);
		m_shadowMap->WaitEndRenderToShadowMap(m_renderContext);
		ChangeToMainRenderTarget(m_renderContext);

		m_renderContext.SetRenderStep(enRenderStep_PreRender);
		//Gbufferに書き込みを行う。
		m_gBuffer->Render(m_renderContext);
	}
	void CGraphicsEngine::DefferdShading(CGameObjectManager* goMgr)
	{
		//ライト情報の更新。
		m_lightManager->Render(m_renderContext);
		//ディファードレンダリングを行う。
		m_defferd->Render(m_renderContext);
	}
	void CGraphicsEngine::ForwardRender(CGameObjectManager* goMgr)
	{
		//レンダリングステップをフォワードレンダリングにする。
		m_renderContext.SetRenderStep(EnRenderStep::enRenderStep_ForwardRender);
		//レンダリングターゲットを設定。
		m_renderContext.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer->GetRenderTarget(EnGBuffer::enGBufferAlbed).GetDSVCpuDescriptorHandle()
		);
		//フォワードレンダリングパス。
		goMgr->ForwardRender(m_renderContext);

		PhysicsWorld().DebugDrawWorld(m_renderContext);
	}
	void CGraphicsEngine::PostRender(CGameObjectManager* goMgr)
	{
		//レンダリングステップをポストレンダリングにする。
		m_renderContext.SetRenderStep(EnRenderStep::enRenderStep_PostRender);

		//ポストエフェクトを掛ける。
		m_postEffect->Render(m_renderContext);

		goMgr->PostRender(m_renderContext);
	}
	void CGraphicsEngine::BeginRender()
	{
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		//コマンドアロケータををリセット。
		m_commandAllocator->Reset();
		//レンダリングコンテキストもリセット。
		m_renderContext.Reset(m_commandAllocator, m_pipelineState);
		//ビューポートを設定。
		//ビューポートを設定。
		m_renderContext.SetViewport(m_viewport);
		//シザリング矩形を設定。
		m_renderContext.SetScissorRect(m_scissorRect);
		//レンダリングターゲットをメインにする。
		//レンダリングターゲットとして利用可能まで待つ。
		m_renderContext.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget.GetRenderTargetTexture().Get());
		//レンダリングターゲットを設定。
		m_renderContext.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVCpuDescriptorHandle());
		const float clearColor[] = { 0.5f,0.5f,0.5f,1.0f };
		m_renderContext.ClearRenderTargetView(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), clearColor);
		m_renderContext.ClearDepthStencilView(m_mainRenderTarget.GetDSVCpuDescriptorHandle(), 1.0f);

		//m_currentFrameBufferRTVHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		//m_currentFrameBufferRTVHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
		////深度ステンシルバッファのディスクリプタヒープの開始アドレスを取得。
		//m_currentFrameBufferDSVHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
		////バックバッファがレンダリングターゲットとして設定可能になるまで待つ。
		//m_renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex]);

		////レンダリングターゲットを設定。
		//m_renderContext.SetRenderTarget(m_currentFrameBufferRTVHandle, m_currentFrameBufferDSVHandle);

		//const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		//m_renderContext.ClearRenderTargetView(m_currentFrameBufferRTVHandle, clearColor);
		//m_renderContext.ClearDepthStencilView(m_currentFrameBufferDSVHandle, 1.0f);

	}
	void CGraphicsEngine::ChangeToMainRenderTarget(RenderContext& rc)
	{
		rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVCpuDescriptorHandle());
	}
	void CGraphicsEngine::EndRender()
	{
		//バックバッファにテクスチャをコピー
		{
			// レンダリングターゲットへの描き込み完了待ち
			//m_renderContext.WaitUntilFinishDrawingToRenderTarget(m_renderTargets[m_frameIndex]);
			m_renderContext.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget.GetRenderTargetTexture().Get());

			m_currentFrameBufferRTVHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
			m_currentFrameBufferRTVHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
			//深度ステンシルバッファのディスクリプタヒープの開始アドレスを取得。
			m_currentFrameBufferDSVHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
			//バックバッファがレンダリングターゲットとして設定可能になるまで待つ。
			m_renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex]);

			//レンダリングターゲットを設定。
			m_renderContext.SetRenderTarget(m_currentFrameBufferRTVHandle, m_currentFrameBufferDSVHandle);

			const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
			m_renderContext.ClearRenderTargetView(m_currentFrameBufferRTVHandle, clearColor);
			m_renderContext.ClearDepthStencilView(m_currentFrameBufferDSVHandle, 1.0f);

			m_copyFullScreenSprite.Draw(m_renderContext, m_camera2D.GetViewMatrix(), m_camera2D.GetProjectionMatrix());
			//レンダリングターゲットへの書き込み完了待ち。
			m_renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex]);
		}

		//レンダリングコンテキストを閉じる。
		m_renderContext.Close();

		//コマンドを実行。
		ID3D12CommandList* ppCommandLists[] = { m_commandList };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame.
		m_swapChain->Present(m_vsyncInterval, 0);

		//描画完了待ち。
		WaitDraw();
	}

	void CGraphicsEngine::ExecuteCommand()
	{
		//// レンダリングターゲットへの描き込み完了待ち
		//m_renderContext.WaitUntilFinishDrawingToRenderTarget(m_renderTargets[m_frameIndex]);

		//レンダリングコンテキストを閉じる。
		m_renderContext.Close();

		//コマンドを実行。
		ID3D12CommandList* ppCommandLists[] = { m_commandList };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
		//描画完了待ち。
		WaitDraw();
	}
}