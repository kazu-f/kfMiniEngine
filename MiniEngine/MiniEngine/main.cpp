#include "stdafx.h"
#include "system/system.h"
#include "Scenes/GameScene.h"
#include "Scenes/DebugScene.h"
#include "Scenes/LightingDemoScene.h"

#define GAME_SCENE 0
#define DEBUG_SCENE 1
#define LIGHTDEMO_SCENE 2

#define START_SCENE LIGHTDEMO_SCENE

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

#if START_SCENE == DEBUG_SCENE
	NewGO<DebugScene>(0);
#elif START_SCENE == LIGHTDEMO_SCENE
	NewGO<LightingDemoScene>(0);
#elif START_SCENE == GAME_SCENE
	NewGO<GameScene>(0);
#endif
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		/*
			次の目標
			エンジンの中で更新処理をまとめて、
			この中で呼び出す。
		*/
		GameEngine().GameUpdate();
		
		
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


