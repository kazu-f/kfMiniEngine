#include "stdafx.h"
#include "system/system.h"
#include "Scenes/GameScene.h"
#include "Scenes/Title/TitleScene.h"
#include "Scenes/DebugScene.h"
#include "Scenes/LightingDemoScene.h"

#define GAME_SCENE 0
#define DEBUG_SCENE 1
#define LIGHTDEMO_SCENE 2

#define START_SCENE DEBUG_SCENE

void SetInitParam(SInitParam& initParam)
{
	initParam.frameBuffer_W = FRAME_BUFFER_W;	//フレームバッファのサイズ(幅)
	initParam.frameBuffer_H = FRAME_BUFFER_H;	//フレームバッファのサイズ(高さ)
	initParam.gameObjectPrioMax = 20;			//ゲームオブジェクトの優先度の最大数。

	initParam.graphicsConf.shadowConf.isEnable = true;		//シャドウマップの有効化フラグ。
	initParam.graphicsConf.shadowConf.shadowMapWidth = 4096;
	initParam.graphicsConf.shadowConf.shadowMapWidth = 4096;
	initParam.graphicsConf.shadowConf.shadowAreas[0] = 2000.0f;		//シャドウマップの有効範囲(近景)
	initParam.graphicsConf.shadowConf.shadowAreas[1] = 4000.0f;		//シャドウマップの有効範囲(中景)
	initParam.graphicsConf.shadowConf.shadowAreas[2] = 20000.0f;	//シャドウマップの有効範囲(遠景)
	//initParam.graphicsConf.shadowConf.depthOffset[0] = 0.01f;
	//initParam.graphicsConf.shadowConf.depthOffset[1] = 0.02f;
	//initParam.graphicsConf.shadowConf.depthOffset[2] = 0.02f;
	initParam.graphicsConf.shadowConf.lightHeight = 5000.0f;
	initParam.graphicsConf.postEffectConf.tonemap.isEnable = false;		//トーンマップ有効化フラグ。
	initParam.graphicsConf.postEffectConf.tonemap.luminance = 0.24f;	//明るさ。
	initParam.graphicsConf.postEffectConf.isBloom = false;	//ブルームの有効化フラグ。
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
	//NewGO<GameScene>(0);
	NewGO<TitleScene>(0);
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


