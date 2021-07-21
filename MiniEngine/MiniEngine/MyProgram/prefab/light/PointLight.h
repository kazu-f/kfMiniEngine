#pragma once
#include "LightBase.h"
#include "graphics/light/LightData.h"

namespace Engine {
	namespace prefab {
		/// <summary>
		/// ポイントライト。
		/// </summary>
		class CPointLight : public CLightBase
		{
		public:
			CPointLight();
			~CPointLight();
			bool StartSub()override final;		//基底クラスのStartから呼び出す。
			void Update()override final;		//更新処理。

		public:	//セット関数。
			/// <summary>
			/// ライトの座標を設定。
			/// </summary>
			void SetPoisition(const Vector3& pos)
			{
				m_light.position = pos;
			}
			/// <summary>
			/// ライトの色を設定。
			/// </summary>
			void SetColor(const Vector3& color)
			{
				m_light.color = color;
			}
			/// <summary>
			/// 減衰パラメータを設定。
			/// </summary>
			/// <param name="attn">
			/// 減衰定数。
			/// <para>xはポイントライトの影響が届く範囲。</para>
			/// <para>yはポイントライトの減衰率に影響を与える。</para>
			/// <para>yが大きくなると、減衰が強くなり、1.0で線形の減衰率になる。</para>
			/// <para>zは未使用。</para>
			/// </param>
			void SetAttn(const Vector3& attn);

		public:	//ゲット関数。
			/// <summary>
			/// ライトの座標を取得。
			/// </summary>
			const Vector3& GetPosition()const
			{
				return m_light.position;
			}
			/// <summary>
			/// 生データの取得。
			/// </summary>
			SPointLight& GetRawData()
			{
				return m_light;
			}
			/// <summary>
			/// このライトをスポットライトにする。
			/// </summary>
			void EnableSpotLight()
			{
				m_light.hasDirection = 1;
			}
			/// <summary>
			/// スポットライトを無効にする。
			/// </summary>
			void DisableSpotLight()
			{
				m_light.hasDirection = 0;
			}
			/// <summary>
			/// スポットライトの照射方向を設定。
			/// </summary>
			/// <param name="dir"></param>
			void SetSpotLightDirection(const Vector3& dir)
			{
				m_light.directionAndAngle.x = dir.x;
				m_light.directionAndAngle.y = dir.y;
				m_light.directionAndAngle.z = dir.z;
			}
			/// <summary>
			/// スポットライトの照射角度を設定。
			/// </summary>
			/// <param name="rad">照射角度(単位:ラジアン)</param>
			void SetSpotLightAngle(float rad)
			{
				m_light.directionAndAngle.w = rad;
			}
			/// <summary>
			/// スポットライトの照射角度を設定。
			/// </summary>
			/// <param name="deg"></param>
			void SetSpotLightAngleDeg(float deg)
			{
				float rad = Math::DegToRad(deg);
				SetSpotLightAngle(rad);
			}

		private:
			SPointLight m_light;
		};
	}
}
