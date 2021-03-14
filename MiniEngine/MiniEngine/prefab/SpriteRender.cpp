#include "stdafx.h"
#include "SpriteRender.h"

namespace Engine {
	namespace prefab {



		bool CSpriteRender::Start()
		{
			//�������ς݁H
			return m_isInited;
		}

		void CSpriteRender::Update()
		{
			//�X�v���C�g�̍X�V�B
			m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
		}

		void CSpriteRender::ForwardRender(RenderContext& rc)
		{
			//3D��Ԃŕ`�悷��B
			if (m_isDraw3D) {
				m_sprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
			}
		}

		void CSpriteRender::PostRender(RenderContext& rc)
		{
			//2D�`�������B
			if (!m_isDraw3D)
			{
				m_sprite.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix());
			}
		}

	}//namespace prefab

}//namespace Engine
