/// <summary>
/// ゲームオブジェクトマネージャー。
/// </summary>

#include "stdafx.h"
#include "GameObjectManager.h"

namespace Engine {
	void CGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}

	void CGameObjectManager::PreUpdate()
	{
		for (auto objList : m_gameObjectListArray) {
			for (auto& obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
	}

	void CGameObjectManager::Update()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
	}

	void CGameObjectManager::PostUpdate()
	{
		for (auto objList : m_gameObjectListArray) {
			for (auto& obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
	}

	void CGameObjectManager::ForwardRender(RenderContext& rc)
	{
		for (auto objList : m_gameObjectListArray) {
			for (auto& obj : objList) {
				obj->ForwardRenderWrapper(rc);
			}
		}
	}

	void CGameObjectManager::PostRender(RenderContext& rc)
	{
		for (auto objList : m_gameObjectListArray) {
			for (auto& obj : objList) {
				obj->PostRenderWrapper(rc);
			}
		}
	}

	void CGameObjectManager::PreRender(RenderContext& rc)
	{
		for (auto objList : m_gameObjectListArray) {
			for (auto& obj : objList) {
				obj->PreRenderWrapper(rc);
			}
		}
	}

	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//バッファ切り替え。
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for (GameObjectList& goList : m_deleteObjectArray[preBufferNo]) {
			for (IGameObject* go : goList) {
				GameObjectPrio prio = go->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);	//削除するオブジェクトの優先度のリスト取得。
				auto it = std::find(goExecList.begin(), goExecList.end(), go);	//リストからオブジェクトのイテレータを取得。
				if (it != goExecList.end()) {		//リストにいる。
					//削除リストから除外された。
					(*it)->m_isRegistDeadList = false;
					if ((*it)->IsNewFromGameObjectManager()) {	//GameObjectManajerで登録したなら。
						delete (*it);
					}
				}
				goExecList.erase(it);	//マネージャーのリストから除外。
			}
			goList.clear();		//削除が終わったから削除リストをまっさらにする。
		}
	}

	void CGameObjectManager::ExecuteFromGameThread()
	{
		//更新系の処理。
		{
			Start();		//初期化。
			PreUpdate();	//事前アップデート。
			Update();		//更新処理。
			PostUpdate();	//遅延アップデート。
		}
		//描画系の処理。
		{
			GraphicsEngine()->PreRenderUpdate();
			GraphicsEngine()->Render(this);
		}

		ExecuteDeleteGameObjects();		//削除処理。
	}

	void CGameObjectManager::Init(int gameObjectPrioMax)
	{
		if (gameObjectPrioMax >= GAME_OBJECT_PRIO_MAX) {
			//最大数よりも多い。
			std::abort();		//とりあえず異常終了させておく。
		}
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}

}	//!<namespace Engine
