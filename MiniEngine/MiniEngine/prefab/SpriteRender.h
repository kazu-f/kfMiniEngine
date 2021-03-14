#pragma once

namespace Engine {
	namespace prefab {
		class CSpriteRender : public IGameObject
		{
		public:
			CSpriteRender() {};
			~CSpriteRender() {};
			/// <summary>
			/// 初期化。
			/// </summary>
			/// <param name="spriteData">初期化データ。</param>
			void Init(const SpriteInitData& spriteData, bool isDraw3D = false)
			{
				m_sprite.Init(spriteData);	//初期化。
				m_isInited = true;			//初期化済み。
				m_isDraw3D = isDraw3D;		//3D空間で描画するか？
			}
		public:
			bool Start()override final;
			void Update()override final;
			void ForwardRender(RenderContext& rc)override final;
			void PostRender(RenderContext& rc)override final;
		public:		//Set関数。
			/// <summary>
			/// 座標を設定。
			/// </summary>
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
			}
			/// <summary>
			/// 回転を設定。
			/// </summary>
			void SetRotation(const Quaternion& rot)
			{
				m_rotation = rot;
			}
			/// <summary>
			/// 拡大率を設定。
			/// </summary>
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
			}
			/// <summary>
			/// ピボットを設定。
			/// </summary>
			/// <param name="pivot">基点。
			/// <para>{ 0.5, 0.5 }が画像の中心が基点。</para>
			/// <para>{ 0.0, 0.0 }で画像の左下。</para>
			/// <para>{ 1.0, 1.0 }で画像の右上。</para>
			/// </param>
			void SetPivot(const Vector2& pivot)
			{
				m_pivot = pivot;
			}
			/// <summary>
			/// 乗算カラーを設定。
			/// </summary>
			void SetMulColor(const Vector4& mulColor)
			{
				m_sprite.SetMulColor(mulColor);
			}

		public:		//Get関数。
			/// <summary>
			/// 座標を取得。
			/// </summary>
			const Vector3& GetPosition()const
			{
				return m_position;
			}
			/// <summary>
			/// 回転を取得。
			/// </summary>
			const Quaternion& GetRotation()const
			{
				return m_rotation;
			}
			/// <summary>
			/// 拡大率を取得。
			/// </summary>
			const Vector3& GetScale()const
			{
				return m_scale;
			}

		private:
			Sprite m_sprite;								//スプライト。
			Vector3 m_position = Vector3::Zero;				//座標。
			Quaternion m_rotation = Quaternion::Identity;	//回転。
			Vector3 m_scale = Vector3::One;					//スケール。
			Vector2 m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット。
			bool m_isInited = false;						//初期化済み？
			bool m_isDraw3D = false;						//3D空間で描画するか？
		};//class CSpriteRender;

	}//namespace prefab
}//namespace Engine
