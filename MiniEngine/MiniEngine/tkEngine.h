#pragma once

#include "util/Stopwatch.h"
#include "graphics/GraphicsConfig.h"

namespace Engine {

	class CGraphicsEngine;
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
		//インスタンスを取得する。
		static TkEngine& Instance()
		{
			static TkEngine instance;
			return instance;
		}
	public:		//Get関数系統
		//ゲームタイム。
		const CGameTime& GetGameTime()
		{
			return m_gameTime;
		}
		//グラフィックエンジン。
		CGraphicsEngine* GetGraphicsEngine()
		{
			return m_graphicsEngine;
		}
		//物理ワールド。
		CPhysicsWorld& GetPhyshicsWorld()
		{
			return m_physicsWorld;
		}
		//サウンドエンジン。
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
	private:
		void GameSleep();

	private:
		CGraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
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
	static inline TkEngine& GameEngine()
	{
		return TkEngine::Instance();
	}
	/// <summary>
	/// ゲームタイムを取得。
	/// </summary>
	static inline const CGameTime& GameTime()
	{
		return GameEngine().GetGameTime();
	}
	/// <summary>
	/// グラフィックエンジンを取得。
	/// </summary>
	static inline CGraphicsEngine* GraphicsEngine()
	{
		return GameEngine().GetGraphicsEngine();
	}

	/// <summary>
	/// 物理ワールドを取得。
	/// </summary>
	static inline CPhysicsWorld& PhysicsWorld()
	{
		return GameEngine().GetPhyshicsWorld();
	}
	/// <summary>
	/// サウンドエンジンを取得。
	/// </summary>
	static inline CSoundEngine& SoundEngine()
	{
		return GameEngine().GetSoundEngine();
	}
}