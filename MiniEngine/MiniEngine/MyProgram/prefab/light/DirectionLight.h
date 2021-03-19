#pragma once
#include "LightBase.h"
#include "graphics/light/LightData.h"

namespace Engine {
	namespace prefab {
		/// <summary>
		/// ディレクションライト。
		/// </summary>
		class CDirectionLight : public CLightBase
		{
		public:
			CDirectionLight();
			~CDirectionLight();
			bool StartSub()override final;
			void Update()override final;
			/// <summary>
			/// ライトの方向を設定。
			/// </summary>
			void SetDirection(const Vector3& direction);
			/// <summary>
			/// ディレクションライトの方向を取得。
			/// </summary>
			const Vector3& GetDirection()const
			{
				return m_light.direction;
			}
			/// <summary>
			/// ライトのカラーを設定。
			/// </summary>
			/// <param name="color">ライトのカラー。</param>
			void SetColor(const Vector4& color);
			/// <summary>
			/// ライトのカラーを取得
			/// </summary>
			const Vector4& GetColor()const
			{
				return m_light.color;
			}
			/// <summary>
			/// 生データの取得。
			/// </summary>
			SDirectionLight& GetRawData()
			{
				return m_light;
			}

		private:
			SDirectionLight m_light;		//!<ライトのデータ。

		};//// class CDirectionLight
	}
}

