#pragma once

#include "Util.h"

namespace Engine {

	template<class T>
	class TNameKey {
		using MyString = std::basic_string < T, std::char_traits<T>, std::allocator<T>>;
		MyString m_name;			//名前。
		unsigned int m_hashCode = 0;
	public:
		TNameKey() {}
		TNameKey( const T* name) 
		{
			Init(name);
		}
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="name">文字列</param>
		void Init(const T* name)
		{
			if (name != nullptr) {
				m_name = name;
				//名前をハッシュ化。
				m_hashCode = Util::MakeHash(name);
			}
		}
		//ハッシュコードを取得。
		unsigned int GetHashCode()const
		{
			return m_hashCode;
		}
		//名前を取得。
		const T* GetName()const
		{
			return m_name.c_str();
		}
	};//class TNameKey

	using NameKey = TNameKey<char>;
	using WNameKey = TNameKey<wchar_t>;
}//namespace Engine