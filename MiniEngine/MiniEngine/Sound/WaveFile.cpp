#include "stdafx.h"
#include "WaveFile.h"

namespace Engine {
	CWaveFile::CWaveFile()
	{
	}
	CWaveFile::~CWaveFile()
	{
		Release();
	}
	bool CWaveFile::Open(const wchar_t* fileName)
	{
		m_filePath = fileName;						//ファイルパスを記録。
		m_filePathHash = Util::MakeHash(fileName);	//ハッシュ値を記録。
		
		m_hmmio = mmioOpenW(const_cast<wchar_t*>(fileName), NULL, MMIO_ALLOCBUF | MMIO_READ);
		if (m_hmmio == NULL)
		{
			ENGINE_MESSAGE_BOX("waveファイルのオープンに失敗しました。filePath : %s\n", fileName);
			ENGINE_WARNING_LOG("Failed mmioOpen");
			//失敗。
			return false;
		}
		MMCKINFO ckIn;           // chunk info. for general use.
		PCMWAVEFORMAT pcmWaveFormat;  // Temp PCM structure to load in.
		memset(&ckIn, 0, sizeof(ckIn));

		if ((0 != mmioDescend(m_hmmio, &m_ckRiff, NULL, 0)))
		{
			ENGINE_MESSAGE_BOX("Failed mmioDescend");
			Release();
			//失敗。
			return false;
		}
		if ((m_ckRiff.ckid != FOURCC_RIFF) ||
			(m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E'))) {
			ENGINE_MESSAGE_BOX("Failed mmioDescend");
			Release();
			//失敗。
			return false;
		}

		// Search the input file for for the 'fmt ' chunk.
		ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
		if (0 != mmioDescend(m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK)) {
			ENGINE_MESSAGE_BOX("Failed mmioDescend");
			Release();
			//失敗。
			return false;
		}

		// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
		// if there are extra parameters at the end, we'll ignore them
		if (ckIn.cksize < static_cast<LONG>(sizeof(PCMWAVEFORMAT))) {
			ENGINE_MESSAGE_BOX("Failed mmioDescend");
			Release();
			//失敗。
			return false;
		}

		// Read the 'fmt ' chunk into <pcmWaveFormat>.
		if (mmioRead(m_hmmio, reinterpret_cast<HPSTR>(&pcmWaveFormat),
			sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat)) {
			ENGINE_MESSAGE_BOX("Failed mmioRead");
			Release();
			//失敗。
			return false;
		}

		// Allocate the waveformatex, but if its not pcm format, read the next
		// word, and thats how many extra bytes to allocate.
		if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
		{
			m_pwfx = reinterpret_cast<WAVEFORMATEX*>(new CHAR[sizeof(WAVEFORMATEX)]);

			// Copy the bytes from the pcm structure to the waveformatex structure
			memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
			m_pwfx->cbSize = 0;
		}
		else
		{
			// Read in length of extra bytes.
			WORD cbExtraBytes = 0L;
			if (mmioRead(m_hmmio, reinterpret_cast<CHAR*>(&cbExtraBytes), sizeof(WORD)) != sizeof(WORD))
			{
				ENGINE_MESSAGE_BOX("Failed mmioRead");
				Release();
				//失敗。
				return false;
			}

			m_pwfx = reinterpret_cast<WAVEFORMATEX*>(new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes]);

			// Copy the bytes from the pcm structure to the waveformatex structure
			memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
			m_pwfx->cbSize = cbExtraBytes;

			// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
			if (mmioRead(m_hmmio, reinterpret_cast<CHAR*>(
				reinterpret_cast<BYTE*>(&(m_pwfx->cbSize)) + sizeof(WORD)),
				cbExtraBytes) != cbExtraBytes) {
				ENGINE_MESSAGE_BOX("Failed mmioRead");
				Release();
				//失敗。
				return false;
			}
		}

		// Ascend the input file out of the 'fmt ' chunk.
		if (0 != mmioAscend(m_hmmio, &ckIn, 0))
		{
			ENGINE_MESSAGE_BOX("Failed mmioAscend");
			Release();
			//失敗。
			return false;
		}
		ResetFile();
		m_dwSize = m_ck.cksize;

		return true;
	}
	void CWaveFile::ResetFile()
	{
		while (!m_isReadEnd) {}	//読み込み中はリセットをしない。

		if (m_hmmio == NULL)
		{
			//NULLが入っている。
			return;
		}
		//Seek to the data
		if (-1 == mmioSeek(m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
			SEEK_SET)) {
			ENGINE_WARNING_LOG("Faild mmioSeek");
			return;
		}

		//Search the input file for the 'data' chunk.
		m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (0 != mmioDescend(m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK)) {
			ENGINE_WARNING_LOG("Failed mmioDescend");
			return;
		}

	}
	void CWaveFile::Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize)
	{
		MMIOINFO mmioinfoIn;	//current status of m_hmmio
		//NULLチェック。
		if (m_hmmio == NULL)
		{
			return;
		}
		if (pBuffer == NULL)
		{
			return;
		}
		//mmioの情報を取得。
		if (0 != mmioGetInfo(m_hmmio, &mmioinfoIn, 0))
		{
			ENGINE_WARNING_LOG("mmioGetInfo");
			return;
		}

		UINT cbDataIn = sizeToRead;
		if (cbDataIn > m_ck.cksize) {
			cbDataIn = m_ck.cksize;
		}
		*currentReadSize = cbDataIn;
		m_ck.cksize -= cbDataIn;

		for (DWORD cT = 0; cT < cbDataIn; cT++) 
		{
			// Copy the bytes from the io to the buffer.
			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) 
			{
				if (0 != mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ)) {
					ENGINE_WARNING_LOG("mmioAdvance");
					return;
				}

				if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) {
					ENGINE_WARNING_LOG("mmioinfoIn.pchNext");
					return;
				}
			}

			// Actual copy.
			*(reinterpret_cast<BYTE*>(pBuffer) + cT) = *(reinterpret_cast<BYTE*>(mmioinfoIn.pchNext));
			mmioinfoIn.pchNext++;
		}

		//if (0 != mmioSetInfo(m_hmmio, &mmioinfoIn, 0)) {
		//	ENGINE_LOG("mmioSetInfo");
		//	return;
		//}

		m_isReadEnd = true;
	}
	void CWaveFile::ReadAsync(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize)
	{
		if (m_isInvokeReadAsyncThread) {
			m_readAsyncThread.join();
			m_isInvokeReadAsyncThread = false;
		}
		m_isReadEnd = false;

		//読み込みスレッドを立てる。
		m_readAsyncThread = std::thread([this, pBuffer, sizeToRead, currentReadSize] {
				this->Read(pBuffer, sizeToRead, currentReadSize);
			}
		);
		m_isInvokeReadAsyncThread = true;
	}
	void CWaveFile::Release()
	{
		if (m_isInvokeReadAsyncThread) {
			m_readAsyncThread.join();
			m_isInvokeReadAsyncThread = false;
		}

		if (m_hmmio != NULL)
		{
			mmioClose(m_hmmio, 0);
			m_hmmio = NULL;
		}
		//解放。
		delete[] m_pwfx;
		m_pwfx = NULL;
	}
}