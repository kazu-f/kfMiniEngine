#include "stdafx.h"
#include "Fade.h"

namespace Engine {

	CFade* CFade::m_instance = nullptr;

	namespace {
		const char* FADE_SPRITE = "Assets/sprite/preset/fade.dds";
	}

	CFade::CFade()
	{
		if (m_instance != nullptr)
		{
			std::abort();
		}
		m_instance = this;
	}

	CFade::~CFade()
	{
		m_instance = nullptr;
	}

	void CFade::Init()
	{
		SpriteInitData spriteData;
		spriteData.m_ddsFilePath[0] = FADE_SPRITE;
		spriteData.m_fxFilePath = Sprite::SPRITE_SHADER_PATH;
		spriteData.m_height = FRAME_BUFFER_H;
		spriteData.m_width = FRAME_BUFFER_W;
		spriteData.m_isAlpha = true;
		//フェードの画像を読み込む。
		m_sprite.Init(spriteData);
		m_sprite.Update(Vector3::Zero, Quaternion::Identity, Vector3::One);
	}

	void CFade::Update()
	{
		const float FRAME_TIME = GameTime().GetFrameDeltaTime() / m_fadeTime;
		switch (m_state)
		{
		case enState_fadeIn:
			m_currentAlpha -= FRAME_TIME;
			if (m_currentAlpha <= 0.0f) {
				m_currentAlpha = 0.0f;
				m_state = enState_idle;
			}
			break;
		case enState_fadeOut:
			m_currentAlpha += FRAME_TIME;
			if (m_currentAlpha >= 1.0f) {
				m_currentAlpha = 1.0f;
				m_state = enState_idle;
			}
			break;
		case enState_idle:
			break;
		}
	}

	void CFade::FadeRender(RenderContext& rc)
	{
		if (m_currentAlpha > 0.0f) {
			m_sprite.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
			m_sprite.Draw(
				rc,
				MainCamera2D().GetViewMatrix(),
				MainCamera2D().GetProjectionMatrix()
			);
		}
	}

}
