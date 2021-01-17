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
		//�O���t�B�b�N�G���W���̏������B
		m_graphicsEngine = new GraphicsEngine();
		g_graphicsEngine = m_graphicsEngine;
		m_graphicsEngine->Init(hwnd, frameBufferWidth, frameBufferHeight);
		//�����G���W���̏������B
		m_physicsWorld.Init();
		//�Q�[���p�b�h�̏������B
		for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
			g_pad[i] = &m_pad[i];
		}
		//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̏������B
		GameObjectManager().Init(20);
	}
	void TkEngine::GameUpdate()
	{

		BeginFrame();

		//	�����G���W���̍X�V�B
		m_physicsWorld.Update(GameTime().GetFrameDeltaTime());
		//�O���t�B�b�N�G���W���̍X�V�B
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
			//���点�鎞��(�~���b)
			DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - currentTime) * 1000);
			
			Sleep(sleepTime);   //���点��B

			//fps�v���̂��߂ɂ�����x�o�ߎ��Ԃ�}��B
			m_sw.Stop();
		}

		m_gameTime.PushFrameDeltaTime(static_cast<float>(m_sw.GetElapsed()));
	}
}