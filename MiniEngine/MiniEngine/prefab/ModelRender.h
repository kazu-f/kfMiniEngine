#pragma once
/// <summary>
/// モデルレンダー
/// </summary>
#include "graphics/Model.h"
#include "graphics/Animation/Animation.h"


namespace Engine {
	namespace prefab {
		/// <summary>
		/// 3Dモデルを表示する機能。
		/// </summary>
		class ModelRender : public IGameObject
		{
		public:
			/// <summary>
			/// 開始処理
			/// </summary>
			/// <returns></returns>
			bool Start() override;
			/// <summary>
			/// 死亡時の処理。
			/// </summary>
			void OnDestroy()override;
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update() override;
			/// <summary>
			/// 事前レンダリング。
			/// </summary>
			void PreRender(RenderContext& rc)override;
			/// <summary>
			/// フォワードレンダリングのパスで呼ばれる処理。
			/// </summary>
			/// <param name="renderContext"></param>
			void ForwardRender(RenderContext& rc) override;

		public:
			/// <summary>
			/// モデルの初期化処理。
			/// </summary>
			/// <param name="modelData">Modelのデータ。</param>
			/// <param name="animClipDatas">アニメーションクリップの初期化データの配列。</param>
			/// <param name="animClipsNum">配列のサイズ。</param>
			void Init(ModelInitData& modelData, AnimClipInitData animClipDatas[] = nullptr,int animClipsNum = 0,int maxInstance = 1)
			{
				//初期化処理を呼び出す。
				m_model.Init(modelData,maxInstance);
				//ファイルパスをメモ。
				m_tkmFilePath = modelData.m_tkmFilePath;
				//アニメーションの初期化データをコピー。
				if (animClipDatas != nullptr)
				{
					for (int i = 0; i < animClipsNum; i++)
					{
						m_animClipInitDatas.push_back(animClipDatas[i]);
					}
				}

				//初期化ステップを開始する。
				m_initStatus = enInitStatus_WaitInitModel;
			}

		public:		//座標などの変更を行う関数。
			/// <summary>
			/// モデルを移動させる。
			/// </summary>
			/// <param name="move">移動量</param>
			void Move(const Vector3& move)
			{
				m_position += move;
			}
			/// <summary>
			/// 座標を取得。
			/// </summary>
			const Vector3& GetPosition() const
			{
				return m_position;
			}
			/// <summary>
			/// モデルを回転させる。
			/// </summary>
			/// <param name="rot">回転量</param>
			void Rotate(const Quaternion& rot)
			{
				//回転を加算する。
				m_rotation *= rot;
			}
			/// <summary>
			/// 座標を設定。
			/// </summary>
			/// <param name="pos">座標</param>
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
			}
			void SetPosition(float x, float y, float z)
			{
				m_position.Set(x, y, z);
			}
			/// <summary>
			/// 回転を設定。
			/// </summary>
			/// <param name="rot"></param>
			void SetRotation(Quaternion rot)
			{
				m_rotation = rot;
			}
			/// <summary>
			/// 拡大率を設定。
			/// </summary>
			/// <param name="scale"></param>
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
			}
			void SetScale(float xScale, float yScale, float zScale)
			{
				m_scale.Set(xScale, yScale, zScale);
			}
			/// <summary>
			/// シャドウキャスターフラグを設定。
			/// </summary>
			/// <remarks>
			/// シャドウキャスターフラグがtrueになっていると、
			/// シャドウマップへのレンダリングが行われる。
			/// </remarks>
			/// <param name="flag"></param>
			void SetShadowCasterFlag(bool flag)
			{
				m_isShadowCaster = flag;
			}
			/// <summary>
			/// シャドウレシーバーフラグを設定。
			/// </summary>
			/// <remarks>
			/// シャドウレシーバーフラグがtrueになっていると、
			/// シャドウマップを参照して、他のオブジェクトからの影が落ちる。
			/// </remarks>
			/// <param name="flag"></param>
			void SetShadowReceiverFlag(bool flag)
			{
				m_model.SetShadowReceiverFlag(flag);
			}

			void UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
			{
				m_model.UpdateInstancingData(pos, rot, scale);
			}
			/// <summary>
			/// 再生するアニメーションを変更する。
			/// </summary>
			/// <param name="animNo">アニメーション番号</param>
			/// <param name="interpolateTime">補完時間</param>
			void PlayAnimation(int animNo, float interpolateTime)
			{
				if (m_animation.IsInited()) {
					m_animation.Play(animNo, interpolateTime);
				}
			}
			/// <summary>
			/// モデルを取得。
			/// </summary>
			/// <returns></returns>
			Model& GetModel()
			{
				return m_model;
			}
			/// <summary>
			/// アニメーションのfootstepの移動量を取得する。
			/// </summary>
			/// <returns></returns>
			const Vector3& GetFootstepMove() const
			{
				if (m_animation.IsInited()) {
					return m_animation.CalsFootstepDeltaInWorldSpace(m_rotation, m_scale);
				}
				return Vector3::Zero;
			}
		private:
			/// <summary>
			/// 初期化ステータス。
			/// </summary>
			enum EnInitStatus {
				enInitStatus_NotCallInitFunc,			//初期化関数がまだ。
				enInitStatus_WaitInitModel,				//モデル初期化待ち。
				enInitStatus_WaitInitSkeleton,			//スケルトンの初期化待ち。
				enInitStatus_WaitInitAnimationClips,	//アニメーションクリップの初期化待ち。
				enInitStatus_Completed,					//初期化完了。
			};
			EnInitStatus m_initStatus = enInitStatus_NotCallInitFunc;	//初期化ステータス。
			Model m_model;		//モデル。
			Vector3 m_position = Vector3::Zero;				//座標。
			Quaternion	m_rotation = Quaternion::Identity;	//回転。
			Vector3 m_scale = Vector3::One;					//拡大率。
			Skeleton m_skeleton;							//スケルトン。
			CAnimation m_animation;							//アニメーション。
			std::string m_tkmFilePath;						//tkmファイルのファイルパス。
			std::vector <AnimClipInitData> m_animClipInitDatas;	//アニメーションクリップの初期化データ。
			std::vector <CAnimationClipPtr> m_animClips;	//アニメーションクリップの配列。
			bool m_isShadowCaster = false;					//シャドウキャスタ―フラグ。
		};
	}
}


