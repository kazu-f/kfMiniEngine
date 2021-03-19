#pragma once

namespace Engine {
	/// <summary>
	/// Utilクラス。
	/// </summary>

	class Util {
	public:
		/// <summary>
		/// 文字列から32bitのハッシュ値を作成。
		/// </summary>
		static int MakeHash(const char* string)
		{
			if (string == nullptr)
			{
				return 0;
			}
			int hash = 0;									//ハッシュ値。
			int l = static_cast<int>(strlen(string));		//文字列の長さ。
			//ハッシュ値を作成。
			for (int i = 0; i < l; i++) {
				hash = hash * 37 + string[i];
			}
			return hash;
		}
		/// <summary>
		/// 文字列から32bitのハッシュ値を作成。
		/// </summary>
		static int MakeHash(const wchar_t* string)
		{
			if (string == nullptr)
			{
				return 0;
			}
			int hash = 0;									//ハッシュ値。
			int l = static_cast<int>(wcslen(string));		//文字列の長さ。
			//ハッシュ値を作成。
			for (int i = 0; i < l; i++) {
				hash = hash * 37 + string[i];
			}
			return hash;
		}
	};

}