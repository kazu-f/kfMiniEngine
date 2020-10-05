#pragma once
/// <summary>
/// ゲームオブジェクトインターフェース。
/// </summary>

namespace Engine {
	class CGameObjectManager;					//ゲームオブジェクトマネージャー。
	typedef unsigned char  GameObjectPrio;		//実行優先度。

	/// <summary>
	/// ゲームオブジェクト。
	/// </summary>
	class IGameObject : Noncopyable
	{
	public:
		IGameObject() :
			m_priority(0),
			m_isStart(false),
			m_isDead(false),
			m_isNewFromGameObjectManager(false)
		{
		}
		virtual ~IGameObject()		//仮想化しないと派生クラスのデストラクタが呼ばれない
		{
		}
	public:		//イベントリスナーのパラメータ
		/// <summary>
		/// イベント
		/// </summary>
		enum EnEvent {
			enEvent_Undef,			//未定義イベント。
			enEvent_Destroy,		//インスタンスが破棄される。
			enBaseClassEventNum,	//基底クラスで定義されているイベントの数。
			enEvent_User,			//これ以降にユーザー定義イベントを作成。

		};

		/// <summary>
		/// イベント発生時のデータ。
		/// </summary>
		struct SEventParam {
			EnEvent eEvent = enEvent_Undef;		//発生しているイベント。
			IGameObject* gameObject = nullptr;	//イベントを通知しているゲームオブジェクトのインスタンス。
		};

		/// <summary>
		/// イベントリスナー。
		/// </summary>
		typedef std::function<void(SEventParam& eventParam)> EventListener;

	public:	//初期化、更新処理など。
		/// <summary>
		/// インスタンスが生成された直後に呼び出される関数。
		/// </summary>
		/// <remarks> コンストラクタが呼び出された直後に呼び出される。 </remarks>
		virtual void Awake() {}
		/// <summary>
		/// Updateの直前で呼ばれる開始処理。
		/// </summary>
		/// <remarks>
		/// この関数がtureを返すと準備完了と判断され、
		/// Update関数が呼ばれだす。trueを返すとそれ以降Start関数は呼ばれない。
		/// 初期化に時間がかかる場合などはfalseを返して、初期化ステップ(?)を使って適切な初期化をする。
		/// 初期化完了判定。
		/// </returns>
		virtual bool Start() { return true; }
		/// <summary>
		/// 更新処理。
		/// </summary>
		virtual void Update() {}
		/// <summary>
		/// 描画処理。
		/// </summary>
		virtual void Draw() {}
		/// <summary>
		/// 削除されるときに呼ばれる。
		/// </summary>
		/// <remarks>デストラクタより前に実行される。</remarks>
		virtual void OnDestroy() {}
		/// <summary>
		/// 実行優先度を取得。
		/// </summary>
		/// <returns>実行優先度。</returns>
		GameObjectPrio GetPriority()const
		{
			return m_priority;
		}

	public:		//SetやGet関係。
		/// <summary>
		/// 死亡フラグを立てる。
		/// </summary>
		/// <remarks>この関数はエンジンの外からは実行しない。</remarks>
		void SetDeadMark()
		{
			m_isDead = true;
		}
		/// <summary>
		/// 死亡判定。
		/// </summary>
		bool IsDead()const
		{
			return m_isDead;
		}
		/// <summary>
		/// Start関数が完了したか？
		/// </summary>
		virtual bool IsStart()const
		{
			return m_isStart;
		}
		/// <summary>
		/// タグを設定。
		/// </summary>
		void SetTags(unsigned int tags)
		{
			m_tags = tags;
		}
		/// <summary>
		/// タグを取得。
		/// </summary>
		unsigned int GetTags()const
		{
			return m_tags;
		}
		/// <summary>
		/// GameObjectManajerで登録したかを設定。
		/// </summary>
		void SetMarkNewFromGameObjectManager()
		{
			m_isNewFromGameObjectManager = true;
		}
		/// <summary>
		/// GameObjectManajerで登録したか？
		/// </summary>
		bool IsNewFromGameObjectManager() const
		{
			return m_isNewFromGameObjectManager;
		}

	public:		//イベントリスナーの処理。
		/// <summary>
		/// リスナーイベントを登録。
		/// </summary>
		/// <param name="listener">イベントリスナー。</param>
		void AddEventListener(EventListener listener)
		{
			m_eventListeners.push_back(listener);
		}
#if 0	//普通に使えないんですがそれは
		/// <summary>
		/// イベントリスナーを破棄。
		/// </summary>
		/// <param name="listener">破棄するイベントリスナー。</param>
		void RemoveEventListener(EventListener listener)
		{
			auto it = std::remove_if(
				m_eventListeners.begin(),
				m_eventListeners.end(),
				[&](EventListener& l) {return l == listener; }
			);

			m_eventListeners.erase(it, m_eventListeners.end());
		}
#endif // 0

	public: //処理のラップ。
		void StartWrapper()
		{
			if (!m_isStart && m_isRegist && !m_isDead && !m_isRegistDeadList) {
				if (Start()) {
					//初期化完了。
					m_isStart = true;
				}
			}
		}
		void UpdateWrapper()
		{
			if (m_isStart && m_isRegist && !m_isDead && !m_isRegistDeadList) {
				Update();
			}
		}
		void DrawWrapper()
		{
			if (m_isStart && m_isRegist && !m_isDead && !m_isRegistDeadList) {
				Draw();
			}
		}
		void OnDestroyWrapper()
		{
			SEventParam param;
			param.eEvent = enEvent_Destroy;
			param.gameObject = this;
			//デストロイイベントをリスナーに通知する。
			for (auto& listener : m_eventListeners) {
				listener(param);
			}

			OnDestroy();
		}

		friend class CGameObjectManager;
	protected:
		GameObjectPrio m_priority;			//!<実行優先度。
		bool m_isStart;						//!<Startの開始フラグ。
		bool m_isDead;						//!<死亡フラグ。
		bool m_isRegistDeadList = false;	//!<死亡リストに積まれている。
		bool m_isNewFromGameObjectManager;	//|<GameObjectManagerでnewされた。
		bool m_isRegist = false;			//!<GameObjectManajerに登録されている？
		unsigned int m_tags = 0;			//!<タグ。
		unsigned int m_nameKey = 0;			//!<名前キー。
		std::list<EventListener> m_eventListeners;		//!<イベントリスナー。
	};	//!<class IGameObject

}	//!<namespace Engine

