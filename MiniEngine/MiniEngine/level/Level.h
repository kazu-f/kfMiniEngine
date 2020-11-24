#pragma once

#include "TklFile.h"

namespace Engine {
	class CMapChip;
	class CMapChipRender;
	/// <summary>
	/// オブジェクトデータ。
	/// </summary>
	struct SLevelObjectData {
		Vector3 position;		//座標。
		Quaternion rotation;	//回転。
		Vector3 scale;			//拡大率。
		const wchar_t* name;	//名前。
		bool isShadowCaster;	//シャドウキャスター
		bool isShadowReceiver;	//シャドウレシーバー
		/// <summary>
		/// 引数で渡したオブジェクト名のオブジェクトか調べる。
		/// </summary>
		/// <param name="objName">調べる名前。</param>
		/// <returns>完全一致でtrueを返す。</returns>
		bool EqualObjectName(const wchar_t* objName)
		{
			return wcscmp(objName, name) == 0;
		}
		/// <summary>
		/// 名前が前方一致するかを調べる。
		/// </summary>
		/// <param name="n">調べる名前。</param>
		/// <returns>前方一致するならtrueを返す。</returns>
		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//名前が長い。不一致。
				return false;
			}
			return wcsncmp(n, name, len) == 0;
		}
	};
	/// <summary>
	/// レベル。
	/// </summary>
	class CLevel
	{
	private:
		using CMapChipPtr = std::unique_ptr<CMapChip>;
		using CMapChipRenderPtr = std::unique_ptr<CMapChipRender>;
	public:
		CLevel();
		~CLevel();
		/// <summary>
		/// レベルを初期化する。
		/// </summary>
		/// <param name="filePath">tklファイルのファイルパス。</param>
		/// <param name="hookFunc">オブジェクトを作成するときの処理をフックするための関数オブジェクト</param>
		/// <remarks>
		/// フックしないならnullptr指定するといい。
		/// 関数オブジェクトがfalseを返すと、オブジェクトの情報から
		/// MapChipクラスのインスタンスが生成される。
		/// オブジェクトの名前などで、特殊なクラスのインスタンスを生成する場合に
		/// MapChipクラスのインスタンスが不要ならtrueを返すと作られなくなる。
		/// </remarks>
		void Init(const char* filePath, std::function<bool(SLevelObjectData& objData)> hookFunc);
	private:
		/// <summary>
		/// マップチップレンダーを作成or描画するオブジェクトの数をインクリメントする。
		/// </summary>
		/// <remarks>
		/// 未登録のオブジェクトが渡されたときは、新たに作成。
		/// 登録済みの場合は描画すべきオブジェクト数がインクリメントされる。
		/// </remarks>
		void CreateMapChipRenderOrAddRenderObject(const SLevelObjectData& objData);
		/// <summary>
		/// ボーン行列の構築。
		/// </summary>
		void BuildBoneMatrices();
	private:
		std::vector<CMapChipPtr> m_mapChipPtrs;			//マップチップの可変長配列。
		std::map<unsigned int, CMapChipRender*> m_mapChipRenderPtrs;	//マップチップレンダーの可変長配列。

		CTklFile m_tklFile;					//tklファイル。
		static const int bone_MAX = 512;	//オブジェクトの最大数。
		using BonePtr = std::unique_ptr<Bone>;
		std::vector<BonePtr> m_bones;		//ボーンの配列。
		std::unique_ptr<Matrix[]> m_boneMatrixs;	//ボーン行列。
		bool m_isInited = false;			//初期化済みかどうか。
	};	//class CLevel;
}	//namespace Engine

