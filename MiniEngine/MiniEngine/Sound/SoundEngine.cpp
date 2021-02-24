#include "stdafx.h"
#include "SoundEngine.h"
#include "WaveFile.h"
#include "SoundSource.h"

#define NUM_PRESETS 30				//プリセットの数。
namespace Engine {

	namespace {
		//マイクロソフトのサンプルから引っ張ってきたサウンドコーン。
		
		//サウンドコーンを指定して、芸術的な効果のためにリスナーに方向性を追加します。
		//リスナーの背後にあるエミッターは、ここでより減衰するように定義されています。
		//LPFカットオフ周波数が低く、
		//まだ少し高いリバーブセンドレベルがあります。
		static const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI * 5.0f / 6.0f, X3DAUDIO_PI * 11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

		//より早くロールオフするようにLFEレベルの距離曲線を指定します
		//すべての非LFEチャンネル、サブウーファーをより劇的に利用します。
		static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
		static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

		//リバーブセンドが増加するようにリバーブセンドレベルの距離カーブを指定します
		//少し距離を置いてから、ロールオフして無音にします。
		//直接チャネルは距離とともにますます減衰し、
		//これには、リバーブとダイレクトのサウンド比を上げる効果があります。
		//距離の認識を強化します。
		static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
		static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

		XAUDIO2FX_REVERB_I3DL2_PARAMETERS PRESET_PARAMS[NUM_PRESETS] =
		{
			XAUDIO2FX_I3DL2_PRESET_FOREST,
			XAUDIO2FX_I3DL2_PRESET_DEFAULT,
			XAUDIO2FX_I3DL2_PRESET_GENERIC,
			XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
			XAUDIO2FX_I3DL2_PRESET_ROOM,
			XAUDIO2FX_I3DL2_PRESET_BATHROOM,
			XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
			XAUDIO2FX_I3DL2_PRESET_STONEROOM,
			XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
			XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
			XAUDIO2FX_I3DL2_PRESET_CAVE,
			XAUDIO2FX_I3DL2_PRESET_ARENA,
			XAUDIO2FX_I3DL2_PRESET_HANGAR,
			XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
			XAUDIO2FX_I3DL2_PRESET_HALLWAY,
			XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
			XAUDIO2FX_I3DL2_PRESET_ALLEY,
			XAUDIO2FX_I3DL2_PRESET_CITY,
			XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
			XAUDIO2FX_I3DL2_PRESET_QUARRY,
			XAUDIO2FX_I3DL2_PRESET_PLAIN,
			XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
			XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
			XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
			XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
			XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
			XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
			XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
			XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
			XAUDIO2FX_I3DL2_PRESET_PLATE,
		};
	}

	CSoundEngine::CSoundEngine()
	{
		memset(m_hx3DAudio, 0, sizeof(m_hx3DAudio));
	}

	CSoundEngine::~CSoundEngine()
	{
		Release();
	}
	void CSoundEngine::Init()
	{
		//Comコンポーネントの初期化。
		//COMのスレッド間の同期をの設定。
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		unsigned int flags = 0;		//フラグ。

		HRESULT hr;

		//XAUDIO2を初期化。
		{
			if (FAILED(hr = XAudio2Create(&m_xAudio2, flags)))
			{
				ENGINE_MESSAGE_BOX(
					"Faild XAudio2Create\n"
					"オーディオデバイスの初期化に失敗しました。"
				);
					return;
			}
			if (FAILED(hr = m_xAudio2->CreateMasteringVoice(&m_masteringVoice)))
			{
				Release();
				ENGINE_MESSAGE_BOX(
					"Faild CreateMesteringVoice\n"
					"オーディオデバイスの初期化に失敗しました。"
				);
				return;
			}
		}
		XAUDIO2_VOICE_DETAILS voiceDetails;
		m_masteringVoice->GetVoiceDetails(&voiceDetails);
		m_masteringVoice->GetChannelMask(&m_channelMask);

		m_nChannels = voiceDetails.InputChannels;

		//リバーブエフェクトを作成。
		{
			flags = 0;
			if (FAILED(hr = XAudio2CreateReverb(&m_reverbEffect, flags)))
			{
				Release();
				ENGINE_MESSAGE_BOX(
					"Faild XAudio2CreateReverb\n"
					"オーディオデバイスの初期化に失敗しました。"
				);
				return;
			}
		}
		//サブミックスボイスを作成。
		{
			XAUDIO2_EFFECT_DESCRIPTOR effects[] = { {m_reverbEffect,TRUE,1} };
			XAUDIO2_EFFECT_CHAIN effectChain = { 1,effects };

			if (FAILED(hr = m_xAudio2->CreateSubmixVoice(&m_submixVoice, voiceDetails.InputChannels,
				voiceDetails.InputSampleRate, 0, 0, NULL, &effectChain)))
			{
				Release();
				ENGINE_MESSAGE_BOX(
					"Faild CreateSubmixVoice\n"
					"オーディオデバイスの初期化に失敗しました。"
				);
				return;
			}
		}
		//デフォルトのFXパラメータを設定。
		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative(&PRESET_PARAMS[0], &native);
		m_submixVoice->SetEffectParameters(0, &native, sizeof(native));
		//3Dオーディオの初期化。
		const float SPEEDFSOUND = X3DAUDIO_SPEED_OF_SOUND;
		m_listener.Position = { 0.0f,0.0f,0.0f };		//オーディオリスナーの座標。
		m_listener.OrientFront = { 0.0f,0.0f,1.0f };	//オーディオリスナーの前方向。
		m_listener.OrientTop = { 0.0f,1.0f,0.0f };		//オーディオリスナーの上方向。

		//初期化完了。
		m_isInited = true;
	}
	void CSoundEngine::Release()
	{
		//ToDO波形データンバンク解放。

		if (m_submixVoice != nullptr)
		{
			m_submixVoice->DestroyVoice();
			m_submixVoice = nullptr;
		}
		if (m_reverbEffect != nullptr)
		{
			m_reverbEffect->Release();
			m_reverbEffect = nullptr;
		}
		if (m_masteringVoice != nullptr)
		{
			m_masteringVoice->DestroyVoice();
			m_masteringVoice = nullptr;
		}		
		if (m_xAudio2 != nullptr)
		{
			m_xAudio2->Release();
			m_xAudio2 = nullptr;
		}
		//COMの解放。
		CoUninitialize();
	}
	IXAudio2SourceVoice* CSoundEngine::CreateXAudio2SourceVoice(CWaveFile* waveFile, bool is3DSound)
	{
		ENGINE_ASSERT(waveFile->GetFormat()->nChannels <= INPUTCHANNELS, "Channel over");
		IXAudio2SourceVoice* pSourceVoice;
		if (is3DSound == false)
		{
			//2Dサウンド。
			if (FAILED(m_xAudio2->CreateSourceVoice(&pSourceVoice, waveFile->GetFormat())))
			{
				ENGINE_WARNING_LOG("Failed CreateSourceVoice");
				return nullptr;
			}
		}
		else {
			//3Dサウンド。
			XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
			sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
			sendDescriptors[0].pOutputVoice = m_masteringVoice;
			sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
			sendDescriptors[1].pOutputVoice = m_submixVoice;

			const XAUDIO2_VOICE_SENDS sendList = { 2,sendDescriptors };
			if (FAILED(m_xAudio2->CreateSourceVoice(&pSourceVoice, waveFile->GetFormat(), 0, 2.0f, NULL, &sendList)))
			{
				ENGINE_WARNING_LOG("Failed CreateSourceVoice");
				return nullptr;
			}
		}

		return pSourceVoice;
	}

	void CSoundEngine::Update()
	{
	}
}
