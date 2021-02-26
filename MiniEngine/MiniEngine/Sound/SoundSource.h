#pragma once

#include "WaveFile.h"

namespace Engine {
	namespace prefab {
		class CSoundSource : public IGameObject
		{
		public:
			CSoundSource();			//コンストラクタ。
			~CSoundSource();		//デストラクタ。
			/// <summary>
			/// 初期化。
			/// </summary>
			/// <remarks>オンメモリ再生向けの初期化。</remarks>
			/// <param name="filePath">ファイルパス。(*.wave)形式</param>
			/// <param name="is3DSound">3Dサウンドか？</param>
			void Init(const wchar_t* filePath, bool is3DSound = false);
			/// <summary>
			/// 初期化。
			/// </summary>
			/// <remarks>オンメモリ再生向けの初期化。こちらの方がちょっとだけ速い。</remarks>
			/// <param name="nameKey">名前キー。</param>
			/// <param name="is3DSound">3Dサウンドか？</param>
			void Init(const WNameKey& nameKey, bool is3DSound = false);
			/// <summary>
			/// 初期化。
			/// </summary>
			/// <remarks>ストリーミング再生向けの初期化。</remarks>
			/// <remarks>
			/// <para>リングバッファにストリーミング読み込みを行って、再生を行っていきます。</para>
			/// <para>一度に読み込まれるデータの最大サイズはbufferingSize。</para>
			/// <para>読み込まれたデータはリングバッファにコピーされていく。</para>
			/// </remarks>
			/// <param name="filePath">ファイルパス。(*.wave)形式。</param>
			/// <param name="is3DSound"></param>
			/// <param name="ringBufferSize"></param>
			/// <param name="bufferingSize"></param>
			void InitStreaming(const wchar_t* filePath,
				bool is3DSound = false,
				unsigned int ringBufferSize = 3 * 1024 * 1024,
				unsigned int bufferingSize = 1024 * 512);
			/// <summary>
			/// 解放。
			/// </summary>
			void Release();
			/// <summary>
			/// 再生。
			/// </summary>
			/// <param name="isLoop">ループ再生か？</param>
			void Play(bool isLoop);
			/// <summary>
			/// 停止。
			/// </summary>
			void Stop()
			{
				m_sourceVoice->Stop();
				m_isPlaying = false;
			}
			/// <summary>
			/// 一時停止。
			/// </summary>
			void Pause()
			{
				m_sourceVoice->Stop();
			}
			/// <summary>
			/// 再生中か？
			/// </summary>
			bool IsPlaying()const
			{
				return m_isPlaying;
			}
			/// <summary>
			/// ボリュームを設定。
			/// </summary>
			/// <param name="vol">ボリューム。</param>
			void SetVolume(float vol)
			{
				m_sourceVoice->SetVolume(vol);
			}
			/// <summary>
			/// ボリュームを取得。
			/// </summary>
			float GetVolume()const
			{
				float vol;
				m_sourceVoice->GetVolume(&vol);
				return vol;
			}
			/// <summary>
			/// 音源の座標を設定。
			/// </summary>
			/// <remarks>
			/// <para>3Dサウンドの時に必要になる。</para>
			/// <para>2Dサウンドでは無視される。</para>
			/// </remarks>
			/// <param name="pos">音源の座標。</param>
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
				if (m_isSetPositionFirst)
				{
					m_lastFramePosition = m_position;
					m_isSetPositionFirst = false;
				}
			}
			/// <summary>
			/// 音源の座標を取得。
			/// </summary>
			const Vector3& GetPosition()const
			{
				return m_position;
			}
			/// <summary>
			/// 音源の移動速度を取得。
			/// </summary>
			const Vector3& GetVelocity()
			{
				return m_velocity;
			}
			/// <summary>
			/// ループフラグを取得。
			/// </summary>
			bool GetLoopFlag() const
			{
				return m_isLoop;
			}
			/// <summary>
			/// ボイスの周波数調整比。
			/// </summary>
			/// <remarks>詳細はIXAudio2SourceVoiceのSetFrequencyRaitoを参照。</remarks>
			/// <param name="raito">周波数比</param>
			void SetFrequencyRetio(float raito)
			{
				if (m_sourceVoice != nullptr)
				{
					m_sourceVoice->SetFrequencyRatio(raito);
				}
			}
			/// <summary>
			/// ソースボイスを取得。
			/// </summary>
			IXAudio2SourceVoice* GetXAudio2SourceVoice()
			{
				return m_sourceVoice;
			}
			/// <summary>
			/// 入力チャンネル数を取得。
			/// </summary>
			int GetNumInputChannel()const
			{
				return m_waveFile->GetFormat()->nChannels;
			}
			/// <summary>
			/// 音の放射角？
			/// </summary>
			FLOAT32* GetEmitterAzimuths()
			{
				return m_emitterAzimuths;
			}
			/// <summary>
			/// 行列係数？を取得。
			/// </summary>
			FLOAT32* GetMatrixCoefficients()
			{
				return m_matrixCoefficients;
			}
			/// <summary>
			/// 設定？を取得。
			/// </summary>
			X3DAUDIO_DSP_SETTINGS* GetDspSettings()
			{
				return &m_dspSettings;
			}
		private:
			void InitCommon();
			//ストリーミング再生中の更新処理。
			void UpdateStreaming();
			//オンメモリ再生中の更新処理。
			void UpdateOnMemory();
			//音を流す。
			void PlayCommon(char* buff, unsigned int bufferSize);
			/// <summary>
			/// ストリーミングバッファリングの開始。
			/// </summary>
			void StartStreamingBuffering();
			/// <summary>
			/// 削除する。
			/// </summary>
			void Remove3DSound();
			/// <summary>
			/// 更新。
			/// </summary>
			void Update()override;

		private:
			enum EnStreamingStatus {
				enStreamingBuffering,		//バッファリング中。
				enStreamingQueueing,		//キューイング中。
			};
			std::shared_ptr<CWaveFile>	m_waveFile;					//波形データ。
			IXAudio2SourceVoice*		m_sourceVoice = nullptr;	//ソースボイス。
			bool						m_isLoop = false;			//ループフラグ。
			bool						m_isPlaying = false;		//再生中フラグ。
			bool						m_isStreaming = false;		//ストリーミング再生か？
			unsigned int				m_streamingBufferSize = 0;	//ストリーミング用のバッファリングサイズ。
			unsigned int				m_currentBufferingSize = 0;	//現在のバッファリングのサイズ。
			unsigned int				m_readStartPos = 0;			//読み込みの開始位置。
			unsigned int				m_ringBufferSize = 0;		//リングバッファのサイズ。
			EnStreamingStatus			m_streamingState = enStreamingBuffering;		//ストリーミングステータス。
			bool						m_is3DSound = false;		//3Dサウンドか？
			Vector3						m_position = Vector3::Zero;	//音源の座標。3Dサウンドの時に必要。
			Vector3						m_lastFramePosition = Vector3::Zero;	//音源の1フレーム前の座標。3Dサウンドの時に必要。
			Vector3						m_velocity = Vector3::Zero;	//速度。3Dサウンドの時に必要。
			FLOAT32 m_emitterAzimuths[INPUTCHANNELS];							//音の放射角？
			FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];		//行列係数。
			X3DAUDIO_DSP_SETTINGS		m_dspSettings;				//設定？
			bool m_isSetPositionFirst = true;						//一番最初のsetPositionか？
			bool m_isAvailable = false;								//インスタンスが利用可能か？
		};//class CSoundSource;

	}//namespace prefab;
}//namespace Engine;

