#pragma once

/// <summary>
/// waveファイル。
/// </summary>

namespace Engine {
	/// <summary>
	/// waveファイルを扱うためのクラス。
	/// </summary>
	/// <remarks>
	/// <para>waveファイルの同期読み込み、非同期読み込みをサポートする。</para>
	/// <para>CWave::Readは同期読み込み、CWaveFile::ReadAsyncは非同期読み込みが行われる。</para>
	/// </remarks>
	class CWaveFile
	{
	public:
		CWaveFile();
		~CWaveFile();
		/// <summary>
		/// waveファイルをオープン。
		/// </summary>
		/// <param name="fileName">ファイル名。</param>
		/// <returns>falseを返したらオープンに失敗。</returns>
		bool Open(const wchar_t* fileName);
		/// <summary>
		/// 波形データを読み込む。
		/// </summary>
		/// <param name="pBuffer">波形データを書き込むバッファ。</param>
		/// <param name="sizeToRead">読み込むデータサイズ</param>
		/// <param name="currentReadSize"></param>
		void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
		/// <summary>
		/// 波形データを非同期読み込み。
		/// </summary>
		/// <param name="pBuffer">波形データを書き込むバッファ。</param>
		/// <param name="sizeToRead">読み込むデータサイズ。</param>
		/// <param name="currentReadSize"></param>
		void ReadAsync(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
		/// <summary>
		/// 読み込み終了判定。
		/// </summary>
		bool IsReadEnd()const
		{
			return m_isReadEnd;
		}
		/// <summary>
		/// 波形データのシーク位置をリセット。
		/// </summary>
		void ResetFile();
		/// <summary>
		/// リリース。
		/// </summary>
		void Release();
		/// <summary>
		/// ファイルサイズを取得。
		/// </summary>
		DWORD GetSize()const
		{
			return m_dwSize;
		}
		/// <summary>
		/// ファイルフォーマットを取得。
		/// </summary>
		WAVEFORMATEX* GetFormat()const
		{
			return m_pwfx;
		}
		/// <summary>
		/// ファイルパスのハッシュ値を取得。
		/// </summary>
		unsigned int GetFilePathHash()const
		{
			return m_filePathHash;
		}
		/// <summary>
		/// 読み込み先のバッファを確保。
		/// </summary>
		void AllocReadBuffer(int size)
		{
			m_readBuffer.reset(new char[size]);
		}
		/// <summary>
		/// 読み込み先のバッファを取得。
		/// </summary>
		char* GetReadBuffer()
		{
			return m_readBuffer.get();
		}
	private:
		std::unique_ptr<char[]> m_readBuffer = nullptr;		//読み込み先のバッファ。
		HMMIO	m_hmmio = nullptr;							//Waveファイルハンドル。
		WAVEFORMATEX* m_pwfx = nullptr;						//Waveファイルのフォーマット定義。
		MMCKINFO	m_ckRiff;								//
		DWORD		m_dwSize = 0;							//Waveファイルのサイズ。
		MMCKINFO	m_ck;									//マルチメディアRIFFチャンク。
		BYTE* m_pbData = nullptr;							//
		BYTE* m_pbDataCur = nullptr;						//
		ULONG m_ulDataSize = 0;								//データサイズ？
		volatile bool	m_isReadEnd = true;					//読み込み終了フラグ。
		std::thread		m_readAsyncThread;					//非同期読み込みスレッド。
		bool			m_isInvokeReadAsyncThread = false;	//
		std::wstring	m_filePath;							//ファイルパス。
		unsigned int	m_filePathHash = 0;					//ファイルパスのハッシュコード。
	};

}

