#include "stdafx.h"
#include "ShadowMap.h"

namespace Engine {
	void ShadowMap::Init(SShadowMapConfig& config)
	{
		m_isEnable = config.isEnable;
		if (!m_isEnable) return;		//������������߂�B

		//�V���h�E�}�b�v�̉𑜓x�̐ݒ�B
		int wh[NUM_SHADOW_MAP][2] = {
			{config.shadowMapWidth,config.lightHeight},					//�ߋ���
			{config.shadowMapWidth >> 1,config.shadowMapHeight >> 1},	//�������B
			{config.shadowMapWidth >> 1,config.shadowMapHeight >> 1}	//�������B
		};
		//�N���A�J���[���쐬�B
		float clearColor[] = {
			1.0f,1.0f,1.0f,1.0f
		};
		//�V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g�쐬�B
		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP;shadowMapNo++) {
			m_shadowMaps[shadowMapNo].Create(
				wh[shadowMapNo][0],		//��
				wh[shadowMapNo][1],		//����
				1,						//
				1,						//
				DXGI_FORMAT_R32_FLOAT,	//�J���[�t�H�[�}�b�g�B
				DXGI_FORMAT_R32_FLOAT,	//�f�v�X�X�e���V���t�H�[�}�b�g�B
				clearColor				//�N���A�J���[�B
			);
		}

		//�萔�o�b�t�@������������B
		m_shadowCb.Init(sizeof(m_shadowCbEntity), nullptr);
	}
	void ShadowMap::RenderToShadowMap(RenderContext& rc)
	{
	}
	void ShadowMap::WaitEndRenderToShadowMap(RenderContext& rc)
	{
	}
	void ShadowMap::Update()
	{
	}
	Vector3 ShadowMap::CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition)
	{
		return Vector3();
	}
}
