#pragma once

#include "Level.h"
#include "prefab/ModelRender.h"

namespace Engine {
	struct SLevelObjectData;
	/// <summary>
	/// マップチップレンダー。
	/// </summary>
	class CMapChipRender  :public IGameObject
	{
		CMapChipRender();
		~CMapChipRender();
		void OnDestroy() override;	//DeleteGOされるときに呼び出される関数。
		void Update() override;		//更新処理。

		/// <summary>
		/// 描画するオブジェクトの数を追加。
		/// </summary>
		void AddRenderObject(const SLevelObjectData& objData)
		{
			m_renderObjDatas.push_back(objData);
		}
		/// <summary>
		/// 初期化処理。全ての描画オブジェクトの追加が終わった後で呼び出す必要がある。
		/// </summary>
		void InitAfterAddAllRenderObjects();
		/// <summary>
		/// スキンモデルレンダーを取得。
		/// </summary>
		prefab::ModelRender* GetModelRender()
		{
			return m_modelRender;
		}
		/// <summary>
		/// 描画オブジェクトデータにクエリを行う。
		/// </summary>
		/// <param name="queryFunc">クエリ関数。</param>
		void QueryRenderObjDatas(std::function<void(const SLevelObjectData& objData)> queryFunc)
		{
			for (const auto& renderObjData : m_renderObjDatas)
			{
				queryFunc(renderObjData);
			}
		}

	private:
		int m_numRenderObject = 0;							//描画するオブジェクトの数。
		std::vector<SLevelObjectData> m_renderObjDatas;		//描画するオブジェクトのデータ配列。
		prefab::ModelRender* m_modelRender = nullptr;		//モデルレンダー。
	};		//class CMapChipRender
}	//namespace Engine

