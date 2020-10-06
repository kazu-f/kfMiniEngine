#pragma once
/// <summary>
/// ライトの基底クラス。
/// </summary>

namespace Engine {
	namespace prefab {
		/// <summary>
		/// ライトの基底クラス。
		/// </summary>
		class CLightBase :public IGameObject
		{
		public:
			bool Start()override final;
			void OnDestroy()override final;
			/// <summary>
			/// 
			/// </summary>
			void SetLightingMaterialIDGroup(unsigned int group)
			{
				m_lightingMaterialIDGroup = group;
			}
			/// <summary>
			/// ライトの影響を与えるマテリアルIDグループを取得。
			/// </summary>
			unsigned int GetLightingMaterialIDGroup()const
			{
				return m_lightingMaterialIDGroup;
			}
	
		private:
			virtual bool StartSub() = 0;
			virtual void OnDestroySub() {};
		private:
			unsigned int m_lightingMaterialIDGroup = 0xFFFFFFFF;		//!<ライトの影響を与える相手を識別するマテリアルIDグループ。0xFFFFFFFFですべてに影響を与える。

		};//// class LightBase

	}//// namespace prefab

}//// namespace Engine


