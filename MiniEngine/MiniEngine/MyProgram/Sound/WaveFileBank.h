#pragma once
/// <summary>
/// 波形データバンク。
/// </summary>

namespace Engine {
	class CWaveFile;
	typedef std::shared_ptr<CWaveFile> CWaveFilePtr;		//波形データのシェアードポインタ。
	typedef std::map<unsigned int, CWaveFilePtr> CWaveFilePtrMap;	//波形データポインタのマップ。

	/// <summary>
	/// 波形データバンク。
	/// </summary>
	/// <remarks>
	/// <para>一度ロードされた波形データをバンクに登録することができる。</para>
	/// <para>登録された波形データは再読み込みを行う必要なしに、バンクから再利用することができる。</para>
	/// </remarks>
	class CWaveFileBank:Noncopyable
	{
	public:
		CWaveFileBank();
		~CWaveFileBank();
		/// <summary>
		/// 波形データを登録。
		/// </summary>
		/// <param name="groupID">グループID。最大値はMAX_GROUP - 1。</param>
		/// <param name="waveFile">ファイルパス。</param>
		void RegistWaveFile(int groupID, CWaveFilePtr waveFile);
		/// <summary>
		/// 引数で指定したファイルパスの波形データがバンクに登録されているか検索。
		/// </summary>
		/// <param name="groupID">グループID。最大値はMAX_GROUP - 1。</param>
		/// <param name="filePath">ファイルパス。</param>
		/// <returns>波形データ。登録されていない場合はNULL。</returns>
		CWaveFilePtr FindWaveFile(int groupID, const wchar_t* filePath);
		/// <summary>
		/// 引数で指定したファイルパスの名前キーがバンクに登録されているか検索。
		/// </summary>
		/// <remarks>こちらのほうが少し早くなる。</remarks>
		/// <param name="groupID">グループID。最大値はMAX_GROUP - 1。</param>
		/// <param name="filePath"></param>
		/// <returns></returns>
		CWaveFilePtr FindWaveFile(int groupID, const WNameKey& nameKey);
		/// <summary>
		/// 波形データをバンクから登録解除。
		/// </summary>
		/// <param name="groupID">グループID。最大値はMAX_GROUP - 1。</param>
		/// <param name="waveFile">波形データ。</param>
		void UnregistWaveFile(int groupID, CWaveFilePtr waveFile);
		/// <summary>
		/// グループ単位で解放。
		/// </summary>
		/// <param name="groupID">グループID。最大値はMAX_GROUP - 1。</param>
		void Release(int groupID);
		/// <summary>
		/// 全てのグループを解放。
		/// </summary>
		void ReleaseAll()
		{
			for (int i = 0; i < MAX_GROUP; i++) {
				Release(i);
			}
		}
	private:
		static const int MAX_GROUP = 256;
		CWaveFilePtrMap m_waveFileMap[MAX_GROUP];		//waveファイルのリスト。
	};//class CWaveFileBank;

}//namespace Engine;

