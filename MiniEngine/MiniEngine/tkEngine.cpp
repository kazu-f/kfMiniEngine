#include "stdafx.h"
#include "tkEngine.h"
#include "graphics/GraphicsEngine.h"
#include "physics/PhysicsWorld.h"

namespace Engine {
	TkEngine* g_engine = nullptr;

	TkEngine::TkEngine()
	{
	}

	TkEngine::~TkEngine()
	{
		if (m_graphicsEngine) {
			delete m_graphicsEngine;
		}
	}
	void TkEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
	{
		//グラフィックエンジンの初期化。
		m_graphicsEngine = new GraphicsEngine();
		g_graphicsEngine = m_graphicsEngine;
		m_graphicsEngine->Init(hwnd, frameBufferWidth, frameBufferHeight);
		//物理エンジンの初期化。
		m_physicsWorld.Init();
		//ゲームパッドの初期化。
		for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
			g_pad[i] = &m_pad[i];
		}
		//ゲームオブジェクトマネージャーの初期化。
		GameObjectManager().Init(20);
	}
	void TkEngine::GameUpdate()
	{

		BeginFrame();

		//	物理エンジンの更新。
		m_physicsWorld.Update(GameTime().GetFrameDeltaTime());
		//グラフィックエンジンの更新。
		m_graphicsEngine->Update();

		GameObjectManager().ExecuteFromGameThread();

		EndFrame();
	}
	void TkEngine::BeginFrame()
	{
		m_sw.Start();

		for (auto& pad : m_pad) {
			pad.BeginFrame();
			pad.Update();
		}

	}
	void TkEngine::EndFrame()
	{
		m_sw.Stop();
		const double MIN_FRAME_TIME = 1.0 / 60.0;
		double currentTime = m_sw.GetElapsed();
		if (currentTime < MIN_FRAME_TIME)
		{
			//眠らせる時間(ミリ秒)
			DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - currentTime) * 1000);
			
			Sleep(sleepTime);   //眠らせる。

			//fps計測のためにもう一度経過時間を図る。
			m_sw.Stop();
		}

		m_gameTime.PushFrameDeltaTime(static_cast<float>(m_sw.GetElapsed()));
	}
}