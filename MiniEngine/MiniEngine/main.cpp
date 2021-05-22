#include "stdafx.h"
#include "system/system.h"
#include "Scenes/GameScene.h"
#include "Scenes/DebugScene.h"
#include "Scenes/LightingDemoScene.h"

#define GAME_SCENE 0
#define DEBUG_SCENE 1
#define LIGHTDEMO_SCENE 2

#define START_SCENE GAME_SCENE

void SetInitParam(SInitParam& initParam)
{
	initParam.frameBuffer_W = FRAME_BUFFER_W;	//フレームバッファのサイズ(幅)
	initParam.frameBuffer_H = FRAME_BUFFER_H;	//フレームバッファのサイズ(高さ)
	initParam.gameObjectPrioMax = 20;			//ゲームオブジェクトの優先度の最大数。

	initParam.graphicsConf.shadowConf.isEnable = true;		//シャドウマップの有効化フラグ。
	//initParam.graphicsConf.shadowConf.depthOffset[0] = 0.01f;
	//initParam.graphicsConf.shadowConf.depthOffset[1] = 0.02f;
	//initParam.graphicsConf.shadowConf.depthOffset[2] = 0.02f;
	initParam.graphicsConf.postEffectConf.isBloom = true;	//ブルームの有効化フラグ。
	initParam.graphicsConf.postEffectConf.isFxaa = true;	//アンチエイリアス有効化フラグ。
}

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	SInitParam initParam;
	SetInitParam(initParam);

	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"), initParam);

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


