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

#include "ap.AndroidNativeActivityGlue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "threaded_app", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "threaded_app", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "threaded_app", __VA_ARGS__))
/*デバッグ ビルドの場合は、このライブラリでデバッグ トレースを常に有効にしてください。*/
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

int engine_init_display(struct engine* engine) {
	// OpenGL ES と EGL の初期化

	/*
	* 目的の構成の属性をここで指定します。
	* 以下で、オンスクリーン ウィンドウと
	* 互換性のある、各色最低 8 ビットのコンポーネントの EGLConfig を選択します
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* ここで、アプリケーションは目的の構成を選択します。このサンプルでは、
	* 抽出条件と一致する最初の EGLConfig を
	* 選択する単純な選択プロセスがあります */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID は、ANativeWindow_setBuffersGeometry() に
	* よって受け取られることが保証されている EGLConfig の属性です。
	* EGLConfig を選択したらすぐに、ANativeWindow バッファーを一致させるために
	* EGL_NATIVE_VISUAL_ID を使用して安全に再構成できます。*/
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;

	// GL の状態を初期化します。
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	return 0;
}

void engine_term_display(struct engine* engine) {
	//ディスプレイに関連付けられている EGL コンテキストの削除
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;

	//入力イベントを処理
	/* 追加  -- ここから --*/

	switch (AInputEvent_getType(event))
	{
	case AINPUT_EVENT_TYPE_MOTION: // モーションイベント
		switch (AInputEvent_getSource(event))
		{
		case AINPUT_SOURCE_TOUCHSCREEN:
			engine-> touchX = AMotionEvent_getX(event, 0);
			engine-> touchY = AMotionEvent_getY(event, 0);
			//engine->touchX = AMotionEvent_getRawX(event, 0);
			//engine->touchY = AMotionEvent_getRawY(event, 0);
			LOGI("Touch[0]: X=%.1f Y=%.1f", engine->touchX, engine->touchY);
		default:
			break;
		}
		break;
	default:
		break;
	}

	/* 追加  -- ここまで --*/
}

void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	//メイン コマンドを処理
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// 現在の状態を保存するようシステムによって要求されました。保存してください。
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// ウィンドウが表示されています。準備してください。
		if (engine->app->window != NULL) {
			engine_init_display(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// ウィンドウが非表示または閉じています。クリーン アップしてください。
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// アプリがフォーカスを取得すると

		break;
	case APP_CMD_LOST_FOCUS:
		// アプリがフォーカスを失うと

		break;
	}
}
