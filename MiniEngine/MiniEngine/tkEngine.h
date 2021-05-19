#pragma once

#include "util/Stopwatch.h"
#include "EngineStruct.h"

namespace Engine {

	class CGraphicsEngine;
	class CPhysicsWorld;
	class CSoundEngine;

	class TkEngine : Noncopyable {
	private:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		TkEngine();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~TkEngine();
	public:
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
		void Init(HWND hwnd, const SInitParam& initParam);
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
		//パッドの取得。
		GamePad& GetPad(int padNo)
		{
			ENGINE_ASSERT(
				padNo < GamePad::CONNECT_PAD_MAX
				|| padNo >= 0
				, "パッド番号が範囲外になっています。"
			);
			return m_pad[padNo];
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
	/// パッドを取得。
	/// </summary>
	static inline const GamePad& Pad(int padNo)
	{
		return GameEngine().GetPad(padNo);
	}
	/// <summary>
	/// グラフィックエンジンを取得。
	/// </summary>
	static inline CGraphicsEngine* GraphicsEngine()
	{
		return GameEngine().GetGraphicsEngine();
	}
	/// <summary>
	/// メインカメラを取得。
	/// </summary>
	static inline Camera& MainCamera()
	{
		return GraphicsEngine()->GetMainCamera();
	}
	/// <summary>
	/// 2Dカメラを取得。
	/// </summary>
	static inline Camera& MainCamera2D()
	{
		return GraphicsEngine()->Get2DCamera();
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