#include "stdafx.h"
#include "LightManager.h"

namespace Engine {

	void CLightManager::Init()
	{
		//�A���r�G���g���C�g�̏������B

		//���C�g�̃p�����[�^�p�̒萔�o�b�t�@���쐬�B
		m_lightParamCB.Init(sizeof(SLightParam), nullptr);
		//�f�B���N�V�������C�g�p�̃X�g���N�`���o�b�t�@���쐬�B
		m_directionLightSB.Init(sizeof(SDirectionLight), MAX_DIRECTION_LIGHT, nullptr);
	}

	void CLightManager::LightUpdate()
	{
		//�f�B���N�V�������C�g�̃X�g���N�`���[�o�b�t�@���X�V�B


	}

	void CLightManager::Render(RenderContext& rc)
	{
		//���C�g�p�����[�^�̃f�[�^���X�V�B
		m_lightParamCB.CopyToVRAM(&m_lightParam);
		//�f�B���N�V�������C�g�̃f�[�^���X�V�B
		m_directionLightSB.Update(&m_rawDirectionLight);
	}

	void CLightManager::SendLightDataToGPU(RenderContext& rc)
	{
		//���ɏ����Ȃ�������B
	}

}	////////namespace Engine
