#pragma once
/// <summary>
/// ゲームオブジェクトマネージャークラス。
/// </summary>

#include "IGameObject.h"
#include "util/Util.h"

namespace Engine {
	class CGameObjectManager : Noncopyable
	{
	private:
		CGameObjectManager() :
			m_gameObjectPriorityMax(0)
		{
		}
		~CGameObjectManager()
		{
		}
		/// <summary>
		/// ゲームオブジェクトの名前キーを作成。
		/// </summary>
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");		//デフォルトの名前キー。
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = Util::MakeHash(objectName);
			}
			return hash;
		}
	public:		//マネージャーの外部から呼び出す処理。
		/// <summary>
		/// インスタンスの取得。
		/// </summary>
		static CGameObjectManager&Instance()
		{
			static CGameObjectManager instance;
			return instance;
		}
		//マネージャーの更新処理。
		void ExecuteFromGameThread();

		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="gameObjectPrioMax">ゲームオブジェクトの優先度の最大値。(255まで)</param>
		void Init(int gameObjectPrioMax);

	public:		//ゲームオブジェクトの追加、削除処理。

		void AddGameObject(GameObjectPrio prio, IGameObject* go, const char* objectName = nullptr)
		{
			(void)objectName;
			if (!go->m_isRegist) {
				//go->Awake();		//作ってなかった。
				unsigned int hash = MakeGameObjectNameKey(objectName);
				m_gameObjectListArray.at(prio).push_back(go);
				go->m_isRegist = true;		//登録済み。
				go->m_priority = prio;		//優先度。
				go->m_isStart = false;		//スタート関数は走っていない。
				go->m_nameKey = hash;		//名前キー。
				if (go->m_isDead) {
					//死亡フラグが立っている。
					//削除リストに入っていたらそこから除去？
					go->m_isDead = false;
				}
			}
		}
		/// <summary>
		/// ゲームオブジェクトの作成。
		/// </summary>
		/// <param name="prio">実行優先順位。</param>
		/// <param name="objectName">オブジェクト名。</param>
		/// <returns>作成したゲームオブジェクトのポインタ。</returns>
		template<class T>
		T* NewGameObject(GameObjectPrio prio, const char* objectName)
		{
			(void*)objectName;
			if (prio >= m_gameObjectPriorityMax){	//優先度の最大数より大きい。
#ifdef _DEBUG
				MessageBoxA(NULL, "ゲームオブジェクトの優先度が大きすぎる。", "Warning!!", MB_OK);
				std::abort();
#endif // DEBUG
			}
			T* newObject = new T();				//インスタンスの作成。
			//newObject->Awake();				//作ってない奴。
			newObject->SetMarkNewFromGameObjectManager();			//NewGameObjectで作られた。
			m_gameObjectListArray.at(prio).push_back(newObject);	//リストに追加。
			unsigned int hash = MakeGameObjectNameKey(objectName);	//名前キーの作成。
			newObject->m_isRegist = true;		//リストに登録されている。
			newObject->m_priority = prio;		//優先度の設定。
			newObject->m_nameKey = hash;		//名前キーの設定。

			return newObject;
		}

		/// <summary>
		///	ゲームオブジェクトの削除
		/// </summary>
		void DeleteGameObject(IGameObject* gameObject)
		{
			if (gameObject != nullptr
				&& gameObject->m_isRegist
			) {
				gameObject->SetDeadMark();		//死亡フラグ。
				gameObject->OnDestroyWrapper();	//削除されるときの処理。
				gameObject->m_isRegist = false;	//リストの登録フラグを外す。
				gameObject->m_isRegistDeadList = true;	//削除リストに登録されている。
				//削除リストに登録する。
				m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
				gameObject = nullptr;	//これいる？
			}
		}
		/// <summary>
		/// ゲームオブジェクトの検索。
		/// </summary>
		/// <remarks>重たいらしい。</remarks>
		/// <param name="objectName">オブジェクト名。</param>
		/// <returns>見つかったオブジェクトのポインタ。</returns>
		template<class T>
		T* FindGameObject(const char* objectName , bool enableError)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {	//優先度。
				for (auto go : goList) {				//リスト。
					if (go->m_nameKey == nameKey) {
						//見つかった。
						T* p = dynamic_cast<T*>(go);
						if (p == nullptr){	//型変換ミス。
#ifdef _DEBUG
							MessageBoxA(NULL, "FindGameObject : 型変換に失敗しました。", "Warning!!", MB_OK);
							throw;
#endif // DEBUG
						}
						return p;
					}
				}
			}
			//見つからなかった。
			if (enableError == true) {
#ifdef _DEBUG
				MessageBoxA(NULL, "FindGO関数に指定された名前のインスタンスが見つからなかった。", "Warrning!!", MB_OK);
#endif // DEBUG		
			}
			return nullptr;
		}
		/// <summary>
		/// ゲームオブジェクトの検索。
		/// </summary>
		/// <remarks>重たい。</remarks>
		/// <param name="objectName">オブジェクトの名前。</param>
		/// <param name="func">boolの値を返す関数。falseを返すとクエリを中断。</param>
		template<class T>
		void FindGameObjects(const char* objectName, std::function<bool(T* go)>func)
		{
			unsigned int nameKey = Util::MakeHash(objectName);		//名前キーの作成。
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//同じ名前のゲームオブジェクトを見つけた。
						T* p = dynamic_cast<T*>(go);		//派生先にキャスト。
						if (func(p) == false) {
							//falseならクエリ中断。
							return;
						}
					}
				}
			}
		}
		/// <summary>
		/// 指定したタグのいずれかが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
		/// </summary>
		/// <param name="tags"></param>
		/// <param name="func"></param>
		void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)> func)
		{
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					unsigned int goTags = go->GetTags();
					if ((goTags & tags) != 0) {
						func(go);
					}
				}
			}
		}

	private:	//ゲームオブジェクトマネージャーの更新処理。
		/// <summary>
		/// ゲームオブジェクトの削除を実行。
		/// </summary>
		void ExecuteDeleteGameObjects();

		void Start();		//スタートを呼び出す。
		void PreUpdate();	//事前更新。
		void Update();		//更新処理を呼び出す。
		void PostUpdate();	//遅延更新。

	public:
		/// <summary>
		/// 描画関係の更新処理はグラフィックエンジンから呼び出す。
		/// </summary>
		void Draw();
		void ForwardRender(RenderContext& rc);
		//void RenderHUD(RenderContext& rc);
		void PreRender(RenderContext& rc);

	private:
		typedef std::list<IGameObject*> GameObjectList;			//!<ゲームオブジェクト*のリスト。
		std::vector<GameObjectList> m_gameObjectListArray;		//!<優先度付きのゲームオブジェクトリスト。
		std::vector<GameObjectList> m_deleteObjectArray[2];		//!<削除するオブジェクトのリスト。
																//!<削除処理を行っている最中にDeleteGameObjectが呼ばれる可能性が高いので、ダブルバッファ化。
		GameObjectPrio				m_gameObjectPriorityMax;	//!<ゲームオブジェクトの優先度の最大数。
		int m_currentDeleteObjectBufferNo = 0;					//!<現在の削除オブジェクトのバッファ番号。
		static const unsigned char	GAME_OBJECT_PRIO_MAX = 255;	//!<ゲームオブジェクトの優先度の最大値。

	};	//!<CGameObjectManajer

	//ゲームオブジェクトマネージャーのインスタンス取得。
	static inline CGameObjectManager& GameObjectManager()
	{
		return CGameObjectManager::Instance();
	}
	/// <summary>
	/// ゲームオブジェクトの検索のヘルパー関数。
	/// </summary>
	/// <remarks>名前の検索が入るため遅い。</remarks>
	/// <param name="objectName">ゲームオブジェクトの名前。</param>
	/// <param name="enableErrorMassage">エラーメッセージを有効にするか？</param>
	/// <returns>見つかったインスタンスのアドレス。見つからない場合nullptr。</returns>
	template<class T> 
	static inline T* FindGO(const char* objectName , bool enableErrorMassage = true)
	{
		return GameObjectManager().FindGameObject<T>(objectName,enableErrorMassage);
	}
	/// <summary>
	/// ゲームオブジェクトの検索のヘルパー関数。
	/// </summary>
	/// <remarks>同名のゲームオブジェクト全てに対して、クエリを行う場合に使用する。</remarks>
	/// <param name="objectName">ゲームオブジェクトの名前。</param>
	/// <param name="func">
	/// <para>ゲームオブジェクトが見つかった時に呼ばれるコールバック関数。</para>
	/// <para>falseを返すとクエリを中断する。</para>
	/// </param>
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func) 
	{
		return GameObjectManager().FindGameObjects<T>(objectName, func);
	}
	/// <summary>
	/// ゲームオブジェクト生成のヘルパー関数。
	/// </summary>
	/// <param name="priority">設定したい優先度。</param>
	/// <param name="objctName">ゲームオブジェクトにつける名前。</param>
	/// <param name="">謎。この引数を入れる必要はないはず...？</param>
	/// <returns>生成したインスタンスのアドレス。</returns>
	template<class T>
	static inline T* NewGO(int priority, const char* objctName = nullptr, typename T::IGameObjectIsBase* = nullptr)
	{
		return GameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objctName);
	}
	/// <summary>
	/// ゲームオブジェクト削除のヘルパー関数。
	/// </summary>
	/// <param name="go">削除するインスタンスのポインタ。</param>
	static inline void DeleteGO(IGameObject* go)
	{
		GameObjectManager().DeleteGameObject(go);
	}
	/// <summary>
	/// ゲームオブジェクト削除のヘルパー関数。
	/// </summary>
	/// <remarks>名前の検索が入るため遅い。</remarks>
	/// <param name="objectName">削除するゲームオブジェクトの名前。</param>
	static inline void DeleteGO(const char* objectName)
	{
		IGameObject* go = FindGO<IGameObject>(objectName);		//検索を行う。
		GameObjectManager().DeleteGameObject(go);
	}
	/// <summary>
	/// ゲームオブジェクトを名前指定で削除。
	/// </summary>
	/// <remarks>名前検索が行われるため遅い。</remarks>
	/// <param name="objectName">削除するゲームオブジェクトの名前。</param>
	static inline void DeleteGOs(const char* objectName)
	{
		//クエリを使って全削除。
		QueryGOs<IGameObject>(objectName, [](auto go) {
			DeleteGO(go);
			return true;
			}
		);
	}
	/// <summary>
	/// ゲームオブジェクトの追加のヘルパー関数
	/// </summary>
	/// <param name="priority">追加するゲームオブジェクト。</param>
	/// <param name="go">優先度。</param>
	/// <param name="objectName">ゲームオブジェクトの名前。</param>
	static inline void AddGO(int priority, IGameObject* go, const char* objectName = nullptr)
	{
		GameObjectManager().AddGameObject(static_cast<GameObjectPrio>(priority), go, objectName);
	}
	/// <summary>
	/// 指定したタグのいずれかが含まれるゲームオブジェクトを検索して、見つかった場合指定されたコールバック関数を呼び出す。
	/// </summary>
	/// <param name="tags">タグ</param>
	/// <param name="func">コールバック関数。</param>
	static inline void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)> func)
	{
		GameObjectManager().FindGameObjectsWithTag(tags, func);
	}

}	//!<namespace Engine

