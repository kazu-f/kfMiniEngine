#include "stdafx.h"
#include "ModelRender.h"

namespace Engine {
	namespace prefab {
		bool ModelRender::Start()
		{
			bool inited = false;
			switch (m_initStatus)
			{
			case enInitStatus_NotCallInitFunc:
				//初期化関数がまだ呼ばれていない。
				break;
			case enInitStatus_WaitInitModel:	//モデルのInit処理待ち。
				if (m_model.IsInited()) {
					std::string skeletonFilePath = m_tkmFilePath;
					int pos = static_cast<int>(skeletonFilePath.find(".tkm"));
					skeletonFilePath.replace(pos, 4, ".tks");
					std::ifstream ifs(skeletonFilePath.c_str());
					if (ifs.is_open()) {
						//tksファイルがある。
						m_skeleton.Init(skeletonFilePath.c_str());
						m_initStatus = enInitStatus_WaitInitSkeleton;
					}
					else {
						//ない。
						m_initStatus = enInitStatus_Completed;
					}
				}
				break;
			case enInitStatus_WaitInitSkeleton:
				if (m_skeleton.IsLoaded())
				{
					m_skeleton.BuildBoneMatrices();
					//スケルトンをモデルと関連付け。。
					m_model.BindSkeleton(m_skeleton);
					if (m_animClipInitDatas.empty() == false)
					{
						//アニメーションのデータがあるか。
						for (auto& initData : m_animClipInitDatas)
						{
							auto animClip = std::make_unique<CAnimationClip>();
							animClip->Load(initData.tkaFilePath.c_str());
							m_animClips.push_back(std::move(animClip));
						}
						m_initStatus = enInitStatus_WaitInitAnimationClips;
					}
					else {
						//ない。
						m_initStatus = enInitStatus_Completed;
					}
				}
				break;
			case enInitStatus_WaitInitAnimationClips:{
				bool isLoaded = true;
				for (auto& animClip : m_animClips) {
					if (animClip->IsLoaded() == false) {
						//アニメーションのロードがまだできていない。
						isLoaded = false;
						break;
					}
				}
				if (isLoaded) {
					//アニメーションクリップが全部ロードできた。
					//キーフレームとかアニメーションイベントの設定とか。
					for (int i = 0; i < static_cast<int>(m_animClips.size()); i++) {
						m_animClips[i]->SetLoopFlag(m_animClipInitDatas[i].isLoop);
						m_animClips[i]->BuildKeyFramesAndAnimationEvents();
					}
					//アニメーションを初期化。
					m_animation.Init(m_skeleton, m_animClips);
					m_initStatus = enInitStatus_Completed;
				}
			}	break;
			
			case enInitStatus_Completed:
				//初期化完了。
				inited = true;
				break;
			}

			return inited;
		}
		void ModelRender::OnDestroy()
		{
			//シャドウキャスター登録を解除する。
			auto& shadowMap = g_graphicsEngine->GetShadowMap();
			shadowMap->RemoveShadowCaster(&m_model);
		}
		void ModelRender::Update()
		{
			if (m_isShadowCaster)
			{
				//シャドウキャスターに登録する。
				auto& shadowMap = g_graphicsEngine->GetShadowMap();
				shadowMap->RegistShadowCaster(&m_model);
			}

			m_model.SendGPUInstancingDatas();

			if (m_animation.IsInited()) {
				//アニメーションを再生。
				m_animation.Progress(GameTime().GetFrameDeltaTime());
			}

			//モデルを更新。
			m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

			if (m_skeleton.IsInited()) {
				//スケルトンを更新。
				if (m_model.IsInstancing()) {
					m_skeleton.Update(g_matIdentity);
				}
				else {
					m_skeleton.Update(m_model.GetWorldMatrix());
				}
			}

		}
		void ModelRender::PreRender(RenderContext& rc)
		{
			m_model.Draw(rc);
		}
		void ModelRender::ForwardRender(RenderContext& renderContext)
		{
		}
	}
}