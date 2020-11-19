#pragma once
/// <summary>
/// tklファイル
/// </summary>
/// <remarks>
/// tklファイルはレベルフォーマット。
/// このクラスはtklファイルを読み込むクラス。
/// </remarks>

namespace Engine {
	class CTklFile
	{
	public:
		/// <summary>
		/// オブジェクト。
		/// </summary>
		struct SObject {
			std::unique_ptr<char[]> name;		//モデルの名前。
			int parentNo = -1;					//親の番号。
			float bindPose[4][3] = { 0.0f };	//バインドポーズ。
			float invBindPose[4][3] = { 0.0f };	//バインドポーズの逆行列。
			int no = 0;							//オブジェクトの番号。
			bool isShadowCaster = false;		//シャドウキャスターフラグ。
			bool isShadowReceiver = false;		//シャドウレシーバーフラグ。

			std::vector<int> intDatas;							//intパラメータ。
			std::vector<float> floatDatas;						//floatパラメータ。
			std::vector<std::unique_ptr<char[]>> charsDatas;	//文字列。
			std::vector<Vector3> vec3Datas;						//vector3のパラメータ。
		};
		/// <summary>
		/// 読み込み処理。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void Load(const char* filePath);
		/// <summary>
		/// オブジェクトに対してクエリを行う。
		/// </summary>
		/// <param name="query"></param>
		void QueryObject(std::function<void(SObject& obj)> query)
		{
			for (auto& obj : m_objects) {
				query(obj);
			}
		}
	private:
		int m_tklVersion = 100;		//tklファイルのバージョン。
		int m_numObject = 0;		//オブジェクトの数。
		std::vector<SObject> m_objects;		//オブジェクトのリスト。
	};
}

