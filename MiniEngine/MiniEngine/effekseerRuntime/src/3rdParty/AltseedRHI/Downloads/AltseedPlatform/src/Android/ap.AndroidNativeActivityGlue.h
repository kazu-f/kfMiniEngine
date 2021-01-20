#pragma once

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

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>

#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

	enum {
		/**
		* アプリのメイン スレッドから取得したコマンドの Looper データ ID です。
		* これは ALooper_pollOnce() からの識別子として返されます。この識別子のデータは、
		* android_poll_source 構造体へのポインターです。
		* これらは android_app_read_cmd()
		* および android_app_exec_cmd() で取得し、処理できます。
		*/
		LOOPER_ID_MAIN = 1,

		/**
		* アプリケーションのウィンドウの AInputQueue から取得したイベントの Looper データ ID です。
		* これは ALooper_pollOnce() からの
		* 識別子として返されます。この識別子のデータは
		* android_poll_source 構造体へのポインターです。これらは android_app の inputQueue オブジェクト
		* から読み取れます。
		*/
		LOOPER_ID_INPUT = 2,

		/**
		* ユーザー定義された ALooper の最初の識別子。
		*/
		LOOPER_ID_USER = 3,
	};

	enum {
		/**
		* メイン スレッドからのコマンド: AInputQueue が変更されました。このコマンドの処理の際、
		* android_app->inputQueue は新しいキュー (または NULL)
		* に更新されます。
		*/
		APP_CMD_INPUT_CHANGED,

		/**
		* メイン スレッドからのコマンド: 新しい ANativeWindow を使用する準備ができました。このコマンドを受信する際、
		* android_app->window に新しいウィンドウ画面が
		* 含まれます。
		*/
		APP_CMD_INIT_WINDOW,

		/**
		* メイン スレッドからのコマンド: 既存の ANativeWindow を終了する必要が
		* あります。このコマンドを受信する際にも android_app->window には引き続き
		* 既存のウィンドウが含まれており、android_app_exec_cmd
		を呼び出した後に    * NULL に設定されます。
		*/
		APP_CMD_TERM_WINDOW,

		/**
		* メイン スレッドからのコマンド: 現在の ANativeWindow のサイズが変更されました。
		* 新しいサイズを使用して再描画してください。
		*/
		APP_CMD_WINDOW_RESIZED,

		/**
		* メイン スレッドからのコマンド: このシステムでは、現在の ANativeWindow が再描画
		* される必要があります。一時的な描画の不具合を回避するためには、android_app_exec_cmd()
		* に渡す前にウィンドウを再描画する必要があります。
		*/
		APP_CMD_WINDOW_REDRAW_NEEDED,

		/**
		* メイン スレッドからのコマンド: ウィンドウのコンテンツ領域が変更 (表示または非表示
		* のソフト入力ウィンドウなどから) されました。新しいコンテンツ領域は、
		* android_app::contentRect にあります。
		*/
		APP_CMD_CONTENT_RECT_CHANGED,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティ ウィンドウが入力フォーカス
		* を取得しました。
		*/
		APP_CMD_GAINED_FOCUS,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティ ウィンドウが入力フォーカス
		* を失いました。
		*/
		APP_CMD_LOST_FOCUS,

		/**
		* メイン スレッドからのコマンド: 現在のデバイス構成が変更されました。
		*/
		APP_CMD_CONFIG_CHANGED,

		/**
		* メイン スレッドからのコマンド: システム メモリが不足しています。
		* メモリの使用量を減らしてください。
		*/
		APP_CMD_LOW_MEMORY,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティが開始されました。
		*/
		APP_CMD_START,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティが再開されました。
		*/
		APP_CMD_RESUME,

		/**
		* メイン スレッドからのコマンド: 必要に応じて後で復元できるように、アプリが
		* 新しい保存状態を生成する必要があります。状態を保存した場合は
		* malloc を割り当て、android_app.savedStateSize のサイズで
		* android_app.savedState に配置します。後で解放
		* されます。
		*/
		APP_CMD_SAVE_STATE,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティが一時停止しました。
		*/
		APP_CMD_PAUSE,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティが停止しました。
		*/
		APP_CMD_STOP,

		/**
		* メイン スレッドからのコマンド: アプリのアクティビティが破棄されており、
		* 続行する前に、アプリ スレッドがクリーン アップして終了するのを待機しています。
		*/
		APP_CMD_DESTROY,
	};


	struct android_poll_source {
		//このソースの識別子は、LOOPER_ID_MAIN または 
		//LOOPER_ID_INPUT です。
		int32_t id;

		// この ident に関連付けられている android_app。
		struct android_app* app;

		//このソースからのデータの標準プロセスを実行するために
		//呼び出す関数。
		void(*process)(struct android_app* app, struct android_poll_source* source);
	};

	struct android_app {
		//適切な場合には、アプリケーションはその状態オブジェクトにポインターを合わせることが
		//できます。
		void* userData;

		//メイン アプリ コマンド (APP_CMD_*) を処理するための関数を入力します
		void(*onAppCmd)(struct android_app* app, int32_t cmd);

		//入力イベントを処理するための関数を入力します。現時点で
		// イベントは既にディスパッチされており、返されるときには完了しています。
		// イベントを処理したことがある場合は 1 を返し、すべての既定のディスパッチについては
		// 0 を返します。
		int32_t(*onInputEvent)(struct android_app* app, AInputEvent* event);

		// このアプリが実行されている ANativeActivity オブジェクト インスタンス。
		ANativeActivity* activity;

		// このアプリが実行されている現在の構成。
		AConfiguration* config;

		// これが作成時に指定された最後のインスタンスの保存の状態です。
		// 状態がない場合は NULL です。必要に応じて使用できます。
		//メモリーは、解放される位置で APP_CMD_RESUME に対して android_app_exec_cmd()
		// を呼び出すまで保持され、savedState は NULL に設定されます。
		// これらの変数は、NULL に初期化される位置で APP_CMD_SAVE_STATE を
		//処理する場合にのみ変更される必要があり、状態を malloc にして
		//情報をここに配置します。そうすることで、後でメモリーが
		//解放されます。
		void* savedState;
		size_t savedStateSize;

		// アプリのスレッドに関連付けられている ALooper。
		ALooper* looper;

		// これが、NULL 以外の場合にアプリがユーザー入力イベントを
		// 受信する入力キューです。
		AInputQueue* inputQueue;

		// これが、NULL 以外の場合にアプリが描画できるウィンドウ画面です。
		ANativeWindow* window;

		// ウィンドウの現在のコンテンツ領域は、ユーザーに対して表示される
		// ウィンドウのコンテンツが配置される場所です。
		ARect contentRect;

		// アプリのアクティビティの現在の状態は、APP_CMD_START、
		// APP_CMD_RESUME、APP_CMD_PAUSE、または APP_CMD_STOP のいずれかです。以下を参照してください。
		int activityState;

		// これは、アプリケーションの NativeActivity の破棄中および
		// アプリ スレッドの完了の待機中はゼロ以外になります。
		int destroyRequested;

		// -------------------------------------------------
		// 以下はグルー コードの "個人" の実装です。

		pthread_mutex_t mutex;
		pthread_cond_t cond;

		int msgread;
		int msgwrite;

		pthread_t thread;

		struct android_poll_source cmdPollSource;
		struct android_poll_source inputPollSource;

		int running;
		int stateSaved;
		int destroyed;
		int redrawNeeded;
		AInputQueue* pendingInputQueue;
		ANativeWindow* pendingWindow;
		ARect pendingContentRect;
	};

	struct saved_state {

	};

	struct engine {
		struct android_app* app;

		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;

		float touchX, touchY;
		struct saved_state state;
	};

	int engine_init_display(struct engine* engine);
	void engine_term_display(struct engine* engine);
	int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
	void engine_handle_cmd(struct android_app* app, int32_t cmd);


#ifdef __cplusplus
}
#endif