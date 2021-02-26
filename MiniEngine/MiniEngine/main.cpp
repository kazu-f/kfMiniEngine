#include "stdafx.h"
#include "system/system.h"
#include "Scenes/GameScene.h"
#include "Scenes/DebugScene.h"




#define USE_UNITY_CHAN 0
#define USE_NINJA 1
#define USE_ROBO 2

#define USE_MODEL 2


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

#if 0
	NewGO<DebugScene>(0);
#else
	NewGO<GameScene>(0);
#endif
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	//auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		/*
			次の目標
			エンジンの中で更新処理をまとめて、
			この中で呼び出す。
		*/
		g_engine->GameUpdate();
		
		
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////



		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		//レンダリング終了。
	}
	return 0;
}


