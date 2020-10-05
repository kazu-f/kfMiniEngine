#pragma once

#include "util/Stopwatch.h"
#include "HID/GamePad.h"

namespace Engine {

	class GraphicsEngine;

	class TkEngine : Noncopyable {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~TkEngine();
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
	private:
		GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。

	public:
		Stopwatch			m_sw;						//タイマークラス。
	};

	extern TkEngine* g_engine;	//TKエンジン。
}