#pragma once

namespace Engine {
	/// <summary>
	/// シェーダーの情報。
	/// <para>主にモデルの初期化などに使う。</para>
	/// <para>頂点シェーダーとピクセルシェーダーの両方を指定する必要がある。</para>
	/// </summary>
	struct SShaderInitData {
		const wchar_t* vsFxFilePath = nullptr;
		const char* vsEntryPointFunc = nullptr;
		const wchar_t* psFxFilePath = nullptr;
		const char* psEntryPointFunc = nullptr;
	};

	class Shader {
	public:
		/// <summary>
		/// ピクセルシェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		void LoadPS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// 頂点シェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		void LoadVS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// コンピュートシェーダーをロード。
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="entryFuncName"></param>
		void LoadCS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// コンパイル済みシェーダーデータを取得。
		/// </summary>
		/// <returns></returns>
		ID3DBlob* GetCompiledBlob() const
		{
			return m_blob;
		}

	private:
		/// <summary>
		/// シェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		/// <param name="shaderModel">シェーダーモデル</param>
		void Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel);
	private:
		ID3DBlob* m_blob = nullptr;	//コンパイル済みのシェーダーデータ。
	};

}