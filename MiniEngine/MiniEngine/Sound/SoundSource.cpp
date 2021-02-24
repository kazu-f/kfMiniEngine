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
			m_dspSettings.SrcChannelCount = INPUTCHANNELS;					//�\�[�X�`�����l�����B
			m_dspSettings.DstChannelCount = SoundEngine().GetNumChannel();	//���݂́H�o�̓`�����l�����B
			m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;		//�s��W���H
			m_dspSettings.pDelayTimes = nullptr;							//�f�B���C�H
			m_dspSettings.DopplerFactor = 1.0f;								//�h�b�v���[���ʁH
			m_dspSettings.LPFDirectCoefficient = 0.821412854f;				//�䐔�l�H
			m_dspSettings.LPFReverbCoefficient = 0.75f;						//���o�[�u�H
			m_dspSettings.ReverbLevel = 0.69114286f;						//���o�[�u���x���H
			m_dspSettings.EmitterToListenerAngle = 0.0f;					//�����̊p�x�H
			m_dspSettings.EmitterToListenerDistance = 10.0f;				//���X�i�[�Ƃ̋����B
			m_dspSettings.EmitterVelocityComponent = 0.0f;					//�ړ����x�B
			m_dspSettings.ListenerVelocityComponent = 0.0f;					//���X�i�[�ړ����x�B
		}
		void CSoundSource::Init(wchar_t* filePath, bool is3DSound)
		{
			if (SoundEngine().IsAvailable() == false)
			{
				//�T�E���h�G���W�������p�s�B
				ENGINE_MESSAGE_BOX("�T�E���h�G���W�������p�ł��܂���B");
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
					//wave�t�@�C���̓ǂݍ��݂Ɏ��s�B
					SoundEngine().GetWaveFileBank().UnregistWaveFile(0, m_waveFile);
					m_waveFile.reset();
					//���f�B
					return;
				}
				m_waveFile->AllocReadBuffer(m_waveFile->GetSize());		//wave�t�@�C���̃T�C�Y���̓ǂݍ��݃o�b�t�@���m�ۂ���B
				SoundEngine().GetWaveFileBank().RegistWaveFile(0, m_waveFile);		//�o�^�B
				unsigned int dummy;
				m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
				m_waveFile->ResetFile();
			}

			//�T�E���h�{�C�X�\�[�X���쐬�B
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
				//�T�E���h�G���W�������p�s�B
				ENGINE_MESSAGE_BOX("�T�E���h�G���W�������p�ł��܂���B");
				return;
			}
			m_isAvailable = false;
			m_waveFile = SoundEngine().GetWaveFileBank().FindWaveFile(0, nameKey);
			if (!m_waveFile) {
				m_waveFile.reset(new CWaveFile);
				m_waveFile->Open(nameKey.GetName());
				SoundEngine().GetWaveFileBank().RegistWaveFile(0, m_waveFile);
				m_waveFile->AllocReadBuffer(m_waveFile->GetSize());		//wave�t�@�C���̃T�C�Y���̓ǂݍ��݃o�b�t�@���m�ۂ���B
				SoundEngine().GetWaveFileBank().RegistWaveFile(0, m_waveFile);
				unsigned int dummy;
				m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
				m_waveFile->ResetFile();
			}
			//�T�E���h�{�C�X�\�[�X���쐬�B
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
