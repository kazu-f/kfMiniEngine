/*
* Copyright (C) 2010 The Android Open Source Project
*
* Apache License Version 2.0 (「本ライセンス」) に基づいてライセンスされます。;
* 本ライセンスに準拠しない場合はこのファイルを使用できません。
* 本ライセンスのコピーは、以下の場所から入手できます。
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* 適用される法令または書面での同意によって命じられない限り、本ライセンスに基づいて頒布されるソフトウェアは、
* 明示黙示を問わず、いかなる保証も条件もなしに現状のまま
* 頒布されます。
* 本ライセンスでの権利と制限を規定した文言ついては、
* 本ライセンスを参照してください。
*
*/


#include "ap.Window_Impl_Android.h"
#include "ap.AndroidNativeActivityGlue.h"
#include "ap.Touch_Impl_Android.h"

/**
* これは、android_native_app_glue を使用しているネイティブ アプリケーション
* のメイン エントリ ポイントです。それ自体のスレッドでイベント ループによって実行され、
* 入力イベントを受け取ったり他の操作を実行したりします。
*/
void android_main(struct android_app* state) {
	ap::Window_Impl_Android* window = new ap::Window_Impl_Android();
	ap::WindowInitializationParameter* windowparam = new ap::WindowInitializationParameter();
	windowparam->UserData[0] = state;
	window->Initialize(*windowparam);
	ap::Touch_Impl_Android* touch = new ap::Touch_Impl_Android(window);

	while (window->DoEvent()) {
		
		window->Update();

		struct engine* engine = &(window->engine);

		float x=0, y=0;
		touch->GetPosition(x, y);
		glClearColor(x/engine->width, y/engine->height, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(engine->display, engine->surface);
	}
}
