#pragma once

namespace Engine {
	namespace prefab {
		/// <summary>
		/// 空を描画するゲームオブジェクト。
		/// </summary>
		/// <remarks>
		/// CubuMapを利用した空の描画を行う。高解像度の空の描画が行える。
		/// </remarks>
		class CSky :public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			CSky();
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~CSky();
			/// <summary>
			/// 開始処理。
			/// </summary>
			bool Start() override final;
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update() override final;
			/// <summary>
			/// 削除時処理。
			/// </summary>
			void OnDestroy() override final;

		public:		//Set関数。
			//空のサイズを設定。
			void SetScale(float scale) 
			{
				m_scale = { scale,scale,scale };
				m_isDirty = true;
			}
			//空のサイズを設定(ベクトル)。
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
				m_isDirty = true;
			}
			//自己発光カラーを設定。
			void SetEmissionColor(const Vector3& emissionColor)
			{
				m_emissionColor = emissionColor;
				m_isDirty = true;
			}
			//空の位置を設定。
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
				m_isDirty = true;
			}
			/// <summary>
			/// 空のキューブマップのファイルパスを設定。
			/// </summary>
			/// <param name="filepath">ファイルパス。</param>
			/// <remarks>
			/// 任意のキューブマップに変更したい場合に、使用する。
			/// 変更がなければ、デフォルトのキューブマップがロードされる。
			/// この関数を利用する場合は、インスタンス作成直後に利用する。
			/// それ以外のタイミングでは、既にキューブマップがロード済みとなり、
			/// 変更ができない。
			/// </remarks>
			/// <example>
			/// キューブマップを変更するサンプルコード。
			/// </example>
			/// <code>
			/// auto sky = NewGO<prefab::CSky>(0);		//空のインスタンスを作成。
			/// sky->SetSkyCubeMapFilePath("hoge.dds");	//キューブマップ差し替え。
			/// </code>
			void SetSkyCubeMapFilePath(const wchar_t filePath)
			{
#ifdef _DEBUG
				if (m_isStart == true) {
					ENGINE_WARNING_LOG("SkyCubeMapのロードが完了しているため"
					"キューブマップの差し替えはできません。"
					"適切なタイミングで関数を利用してください。")
				}
#endif
				m_skyCubeMapFilePath = filePath;
			}
			/// <summary>
			/// 空のモデルのファイルパスを設定。
			/// </summary>
			/// <param name="filePath">ファイルパス。</param>
			/// <remarks>
			/// 任意の空のモデルに変更したい場合に使用する。
			/// 変更がなければ、デフォルトの空のモデルがロードされる。
			/// 関数を利用する場合は、インスタンスを作成した直後に利用する。
			/// それ以外のタイミングでは、既に空のモデルがロード済みとなり、
			/// 変更することができない。
			/// </remarks>
			/// <example>
			/// モデルを変更するサンプルコード。
			/// </example>
			/// <code>
			/// auto sky = NewGO<prefab::CSky>(0);
			/// sky->SetSkyModelFilePath("hoge.tkm");
			/// </code>
			void SetSkyModelFilePath(const char* filePath) 
			{
#ifdef _DEBUG
				if (m_isStart == true) {
					ENGINE_WARNING_LOG("空のモデルのロードが完了しているため"
						"空のモデルの差し替えはできません。"
						"適切なタイミングで関数を利用してください。");
				}
#endif
				m_skyModelFilePath = filePath;
			}
		private:
			prefab::ModelRender* m_modelRender = nullptr;			//モデル描画のゲームオブジェクト。
			Texture m_skyCube;										//スカイキューブマップのSRV。
			Vector3 m_scale = { 8000.0f,8000.0f,8000.0f };			//空の拡大率。
			Vector3 m_position = Vector3::Zero;						//座標。
			Vector3 m_emissionColor = Vector3::Zero;				//自己発光カラー。
			bool m_isDirty = false;									//ダーティフラグ。
			std::wstring m_skyCubeMapFilePath = L"Assets/modelData/preset/skyCubeMap.dds";		//空のキューブマップのファイルパス。
			std::string m_skyModelFilePath = "Assets/modelData/preset/sky.tkm";					//空のtkmファイルのファイルパス。
			ModelInitData m_modelData;
		};//class CSky;

	}//namespace prefab;
}//namespace Engine;

