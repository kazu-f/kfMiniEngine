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

		void CSpriteRender::Init(const char* FilePath, float w, float h, bool isDraw3D)
		{
			//初期化データを作成する。
			SpriteInitData spriteData;
			spriteData.m_fxFilePath = Sprite::SPRITE_SHADER_PATH;
			spriteData.m_ddsFilePath[0] = FilePath;
			spriteData.m_width = w;
			spriteData.m_height = h;
			m_sprite.Init(spriteData,isDraw3D);
			m_isDraw3D = isDraw3D;
			m_isInited = true;				//初期化済み。
		}

		void CSpriteRender::ForwardRender(RenderContext& rc)
		{
			//3D空間で描画する。
			if (m_isDraw3D) {
				m_sprite.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
			}
		}

		void CSpriteRender::PostRender(RenderContext& rc)
		{
			//2D描画をする。
			if (!m_isDraw3D)
			{
				m_sprite.Draw(rc, MainCamera2D().GetViewMatrix(), MainCamera2D().GetProjectionMatrix());
			}
		}

	}//namespace prefab

}//namespace Engine
