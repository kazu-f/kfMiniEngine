#pragma once

#include "util/Stopwatch.h"
#include "HID/GamePad.h"

namespace Engine {

	class GraphicsEngine;

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
		const CGameTime& GetGameTime()
		{
			return m_gameTime;
		}

	private:
		GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
		CGameTime			m_gameTime;					//ゲームタイム。
	public:
		Stopwatch			m_sw;						//タイマークラス。
	};

	extern TkEngine* g_engine;	//TKエンジン。
	/// <summary>
	/// ゲームタイムを取得。
	/// </summary>
	static inline const CGameTime& GameTime()
	{
		return g_engine->GetGameTime();
	}
}