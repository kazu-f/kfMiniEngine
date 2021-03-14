#include "stdafx.h"
#include "SpriteRender.h"

namespace Engine {
	namespace prefab {



		bool CSpriteRender::Start()
		{
			//初期化済み？
			return m_isInited;
		}

		void CSpriteRender::Update()
		{
			//スプライトの更新。
			m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
		}

		void CSpriteRender::ForwardRender(RenderContext& rc)
		{
			//3D空間で描画する。
			if (m_isDraw3D) {
				m_sprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
			}
		}

		void CSpriteRender::PostRender(RenderContext& rc)
		{
			//2D描画をする。
			if (!m_isDraw3D)
			{
				m_sprite.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix());
			}
		}

	}//namespace prefab

}//namespace Engine
