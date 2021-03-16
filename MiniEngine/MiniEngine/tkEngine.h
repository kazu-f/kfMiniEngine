#pragma once

#include "util/Stopwatch.h"
#include "graphics/GraphicsConfig.h"

namespace Engine {

	class GraphicsEngine;
	class CPhysicsWorld;
	class CSoundEngine;

	class TkEngine : Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		TkEngine();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~TkEngine();
		/// <summary>
		/// 解放処理。
		/// </summary>
		void Release();
		/// <summary>
		/// ゲームの処理。
		/// </summary>
		void GameUpdate();
		/// <summary>
		/// フレームの開始時に呼ばれる処理。
		/// </summary>
		void BeginFrame();
		/// <summary>
		/// フレームの終了時に呼ばれる処理。
		/// </summary>
		void EndFrame();
		/// <summary>
		/// ゲームエンジンの初期化。
		/// </summary>
		void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	public:		//Get関数系統
		static TkEngine& Instance()
		{
			static TkEngine instance;
			return instance;
		}

		const CGameTime& GetGameTime()
		{
			return m_gameTime;
		}
		CPhysicsWorld& GetPhyshicsWorld()
		{
			return m_physicsWorld;
		}
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
	private:
		void GameSleep();

	private:
		GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
		CPhysicsWorld m_physicsWorld;					//物理ワールド。
		CSoundEngine m_soundEngine;						//サウンドエンジン。
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
		CGameTime			m_gameTime;					//ゲームタイム。
	public:
		Stopwatch			m_sw;						//タイマークラス。
	};

	/// <summary>
	/// エンジンを取得。
	/// </summary>
	static inline TkEngine& GetEngine()
	{
		return TkEngine::Instance();
	}
	/// <summary>
	/// ゲームタイムを取得。
	/// </summary>
	static inline const CGameTime& GameTime()
	{
		return GetEngine().GetGameTime();
	}
	/// <summary>
	/// 物理ワールドを取得。
	/// </summary>
	static inline CPhysicsWorld& PhysicsWorld()
	{
		return GetEngine().GetPhyshicsWorld();
	}

	static inline CSoundEngine& SoundEngine()
	{
		return GetEngine().GetSoundEngine();
	}
}