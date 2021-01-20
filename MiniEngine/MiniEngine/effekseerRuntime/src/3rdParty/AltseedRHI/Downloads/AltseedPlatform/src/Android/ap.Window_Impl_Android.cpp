#include "ap.AndroidNativeActivityGlue.h"
#include "ap.Window_Impl_Android.h"

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
//一部改変

namespace ap
{
	Window_Impl_Android::Window_Impl_Android()
	{

	}

	Window_Impl_Android::~Window_Impl_Android()
	{

	}

	ErrorCode Window_Impl_Android::Initialize(const WindowInitializationParameter& parameter)
	{
		struct android_app* state =(struct android_app*) parameter.UserData[0];

		memset(&engine, 0, sizeof(engine));
		state->userData = &engine;
		state->onAppCmd = engine_handle_cmd;
		state->onInputEvent = engine_handle_input;
		engine.app = state;

		if (state->savedState != NULL) {
			// 以前の保存状態を復元してください。
			engine.state = *(struct saved_state*)state->savedState;
		}

		return ErrorCode::OK;
	}

	bool Window_Impl_Android::DoEvent()
	{
		struct android_app* state = engine.app;
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident = ALooper_pollAll(0, NULL, &events,
			(void**)&source)) >= 0) {

			// このイベントを処理します。
			if (source != NULL) {
				source->process(state, source);
			}


			// 終了するかどうか確認します。
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return false;
			}
		}
		return true;
	}

	void Window_Impl_Android::Update()
	{
		if (engine.display == NULL) {
			// ディスプレイがありません。
			// エラー処理がいる？
			return;
		}
	}
}