#include "stdafx.h"
#include "SoundEngine.h"
#include "SoundSource.h"

namespace Engine {
	namespace prefab {
		CSoundSource::CSoundSource()
		{
			memset(m_emitterAzimuths, 0, sizeof(m_emitterAzimuths));
			memset(m_matrixCoefficients, 0, sizeof(m_matrixCoefficients));
		}
		CSoundSource::~CSoundSource()
		{
			Release();
		}
		void CSoundSource::InitCommon()
		{
			m_dspSettings.SrcChannelCount = INPUTCHANNELS;					//ソースチャンネル数。
			m_dspSettings.DstChannelCount = SoundEngine().GetNumChannel();	//現在の？出力チャンネル数。
			m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;		//行列係数？
			m_dspSettings.pDelayTimes = nullptr;							//ディレイ？
			m_dspSettings.DopplerFactor = 1.0f;								//ドップラー効果？
			m_dspSettings.LPFDirectCoefficient = 0.821412854f;				//謎数値？
			m_dspSettings.LPFReverbCoefficient = 0.75f;						//リバーブ？
			m_dspSettings.ReverbLevel = 0.69114286f;						//リバーブレベル？
			m_dspSettings.EmitterToListenerAngle = 0.0f;					//何かの角度？
			m_dspSettings.EmitterToListenerDistance = 10.0f;				//リスナーとの距離。
			m_dspSettings.EmitterVelocityComponent = 0.0f;					//移動速度。
			m_dspSettings.ListenerVelocityComponent = 0.0f;					//リスナー移動速度。
		}
		void CSoundSource::Init(const wchar_t* filePath, bool is3DSound)
		{
			if (SoundEngine().IsAvailable() == false)
			{
				//サウンドエンジンが利用不可。
				ENGINE_MESSAGE_BOX("サウンドエンジンが利用できません。");
				return;
			}
			m_isAvailable = false;
			m_waveFile = SoundEngine().GetWaveFileBank().FindWaveFile(0, filePath);
			if (!m_waveFile)
			{
				m_waveFile.reset(new CWaveFile);
				bool result = m_waveFile->Open(filePath);
				if (result == false)
				{
					//waveファイルの読み込みに失敗。
					SoundEngine().GetWaveFileBank().UnregistWaveFile(0, m_waveFile);
					m_waveFile.reset();
					//中断。
					return;
				}
				m_waveFile->AllocReadBuffer(m_waveFile->GetSize());		//waveファイルのサイズ分の読み込みバッファを確保する。
				SoundEngine().GetWaveFileBank().RegistWaveFile(0, m_waveFile);		//登録。
				unsigned int dummy;
				m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
				m_waveFile->ResetFile();
			}

			//サウンドボイスソースを作成。
			m_sourceVoice = SoundEngine().CreateXAudio2SourceVoice(m_waveFile.get(), is3DSound);
			if (is3DSound) {
				SoundEngine().Add3DSoundSource(this);
			}
			InitCommon();

			m_is3DSound = is3DSound;
			m_isAvailable = true;
		}
		void CSoundSource::Init(const WNameKey& nameKey, bool is3DSound)
		{
			if (SoundEngine().IsAvailable() == false)
			{
				//サウンドエンジンが利用不可。
				ENGINE_MESSAGE_BOX("サウンドエンジンが利用できません。");
				return;
			}
			m_isAvailable = false;
			m_waveFile = SoundEngine().GetWaveFileBank().FindWaveFile(0, nameKey);
			if (!m_waveFile) {
				m_waveFile.reset(new CWaveFile);
				m_waveFile->Open(nameKey.GetName());
				SoundEngine().GetWaveFileBank().RegistWaveFile(0, m_waveFile);
				m_waveFile->AllocReadBuffer(m_waveFile->GetSize());		//waveファイルのサイズ分の読み込みバッファを確保する。
				SoundEngine().GetWaveFileBank().RegistWaveFile(0, m_waveFile);
				unsigned int dummy;
				m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
				m_waveFile->ResetFile();
			}
			//サウンドボイスソースを作成。
			m_sourceVoice = SoundEngine().CreateXAudio2SourceVoice(m_waveFile.get(), is3DSound);
			if (is3DSound) {
				SoundEngine().Add3DSoundSource(this);
			}
			InitCommon();

			m_is3DSound = is3DSound;
			m_isAvailable = true;
		}

		void CSoundSource::InitStreaming(const wchar_t* filePath, bool is3DSound, unsigned int ringBufferSize, unsigned int bufferingSize)
		{
			if (SoundEngine().IsAvailable() == false) {
				//サウンドエンジンが利用不可。
				ENGINE_MESSAGE_BOX("サウンドエンジンが利用できません。");
				return;
			}
			m_isAvailable = false;
			//ストリーミングはCWaveFileの使いまわしはできない。
			m_waveFile.reset(new CWaveFile);
			m_waveFile->Open(filePath);

			m_isStreaming = true;
			m_streamingBufferSize = bufferingSize;
			m_waveFile->AllocReadBuffer(ringBufferSize);
			m_ringBufferSize = ringBufferSize;
			m_readStartPos = 0;
			m_currentBufferingSize = 0;
			//サウンドボイスソースを作成。
			m_sourceVoice = SoundEngine().CreateXAudio2SourceVoice(m_waveFile.get(), is3DSound);
			if (is3DSound) {
				SoundEngine().Add3DSoundSource(this);
			}
			InitCommon();

			m_is3DSound = is3DSound;
			m_isAvailable = true;
		}
		void CSoundSource::Release()
		{
			if (m_isStreaming) {
				if (m_waveFile) {
					m_waveFile->Release();
				}
			}
			if (m_sourceVoice != nullptr) {
				m_sourceVoice->DestroyVoice();
				m_sourceVoice = nullptr;
			}
			Remove3DSound();
			DeleteGO(this);
		}
		void CSoundSource::PlayCommon(char* buff, unsigned int bufferSize)
		{
			XAUDIO2_BUFFER buffer = { 0 };
			buffer.pAudioData = reinterpret_cast<BYTE*>(buff);
			buffer.Flags = XAUDIO2_END_OF_STREAM;	// tell the source voice not to expect any data after this buffer
			buffer.AudioBytes = bufferSize;
			if (m_sourceVoice != nullptr && bufferSize > 0) {
				//再生開始。
				if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer))) {
					Release();
					ENGINE_LOG("Failed m_sourceVoice->SubmitSourceBuffer");
					return;
				}
			}
		}
		void CSoundSource::StartStreamingBuffering()
		{
			char* readStartBuff = m_waveFile->GetReadBuffer();
			m_readStartPos += m_currentBufferingSize;
			if (m_readStartPos + m_streamingBufferSize >= m_ringBufferSize) {
				//リングバッファのサイズを超える。
				//循環する。
				m_readStartPos = 0;
			}
			m_waveFile->ReadAsync(&readStartBuff[m_readStartPos], m_streamingBufferSize, &m_currentBufferingSize);
			m_streamingState = enStreamingBuffering;
		}
		void CSoundSource::Play(bool isLoop)
		{
			if (m_isAvailable == false) {
				return;
			}
			if (m_isPlaying) {
				//再生中のものを再開する。
				m_sourceVoice->Start(0);
			}
			else {
				if (m_isStreaming) {
					//バッファリング開始。
					m_waveFile->ResetFile();
					StartStreamingBuffering();
					m_sourceVoice->Start(0, 0);
				}
				else {
					m_sourceVoice->FlushSourceBuffers();
					m_sourceVoice->Start(0);
					PlayCommon(m_waveFile->GetReadBuffer(), m_waveFile->GetSize());
				}
				m_isPlaying = true;
			}
			m_isLoop = isLoop;
		}
		void CSoundSource::UpdateStreaming()
		{
			if (!m_isPlaying) {
				return;
			}
			if (m_streamingState == enStreamingBuffering) {
				//バッファリング中。
				if (m_waveFile->IsReadEnd()) {
					//バッファリングが終わった。
					m_streamingState = enStreamingQueueing;
				}
			}
			if (m_streamingState == enStreamingQueueing) {
				//キューイング中。
				XAUDIO2_VOICE_STATE state;
				m_sourceVoice->GetState(&state);
				if (state.BuffersQueued <= 2) {		//キューイングされているバッファが2以下になったらキューイング。
					char* buff = m_waveFile->GetReadBuffer();
					PlayCommon(&buff[m_readStartPos], m_currentBufferingSize);
					if (m_currentBufferingSize == 0) {
						//読み込んだサイズが0なら末端まで読み込みが終わった。
						if (m_isLoop) {
							//ループする？
							//waveファイルの読み込み位置をリセットしてバッファリング再開。
							m_waveFile->ResetFile();
							StartStreamingBuffering();
						}
						else {
							//ループしない場合はキューが空になった時点で再生終了。
							if (state.BuffersQueued == 0) {
								//再生終了。
								m_isPlaying = false;
								DeleteGO(this);
								Remove3DSound();
							}
						}
					}
					else {
						//次をバッファリングする。
						StartStreamingBuffering();
					}
				}
			}
		}
		void CSoundSource::Remove3DSound()
		{
			if (m_is3DSound) {
				SoundEngine().Remove3DSoundSource(this);
				m_is3DSound = false;
			}
		}
		void CSoundSource::UpdateOnMemory()
		{
			if (!m_isPlaying) {
				return;
			}
			XAUDIO2_VOICE_STATE state;

			m_sourceVoice->GetState(&state);
			if (state.BuffersQueued <= 0) {
				m_isPlaying = false;
				if (m_isLoop) {
					//ループ。
					Play(m_isLoop);
				}
				else {
					DeleteGO(this);
					Remove3DSound();
				}
			}
		}
		void CSoundSource::Update()
		{
			if (m_isAvailable == false) {
				return;
			}

			if (m_isStreaming) {
				//ストリーミング再生中の更新。
				UpdateStreaming();
			}
			else {
				//オンメモリ再生中の更新処理。
				UpdateOnMemory();
			}
			if (m_is3DSound == true) {
				//音源の移動速度を更新。
				m_velocity.Subtract(m_position, m_lastFramePosition);
				m_velocity.Div(GameTime().GetFrameDeltaTime());
				m_lastFramePosition = m_position;
			}
		}
	}
}
