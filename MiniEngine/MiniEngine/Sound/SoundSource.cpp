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
		void CSoundSource::Init(wchar_t* filePath, bool is3DSound)
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

		void CSoundSource::InitStreaming(wchar_t* filePath, bool is3DSound, unsigned int ringBufferSize, unsigned int bufferingSize)
		{
		}
		void CSoundSource::Release()
		{
		}
		void CSoundSource::Play(bool isLoop)
		{
		}
		void CSoundSource::UpdateStreaming()
		{
		}
		void CSoundSource::UpdateOnMemory()
		{
		}
		void CSoundSource::PlayCommon(char* buff, unsigned int bufferSize)
		{
		}
		void CSoundSource::StartStreamingBuffering()
		{
		}
		void CSoundSource::Remove3DSound()
		{
		}
		void CSoundSource::Update()
		{
		}
	}
}
