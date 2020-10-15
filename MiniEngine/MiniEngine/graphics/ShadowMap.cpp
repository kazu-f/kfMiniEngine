#include "stdafx.h"
#include "ShadowMap.h"

namespace Engine {
	void CShadowMap::Init(SShadowMapConfig& config)
	{
		m_isEnable = config.isEnable;
		if (!m_isEnable) return;		//������������߂�B

		//�V���h�E�}�b�v�͈̔͂�ݒ肷��B
		for (int i = 0; i < NUM_SHADOW_MAP; i++)
		{
			m_shadowAreas[i] = config.shadowAreas[i];
		}

		//�V���h�E�}�b�v�̉𑜓x�̐ݒ�B
		int wh[NUM_SHADOW_MAP][2] = {
			{config.shadowMapWidth,config.lightHeight},					//�ߋ���
			{config.shadowMapWidth,config.shadowMapHeight >> 1},		//�������B
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
				DXGI_FORMAT_D32_FLOAT,	//�f�v�X�X�e���V���t�H�[�}�b�g�B
				clearColor				//�N���A�J���[�B
			);
		}

		//�萔�o�b�t�@������������B
		m_shadowCb.Init(sizeof(m_shadowCbEntity), nullptr);
	}
	void CShadowMap::RenderToShadowMap(RenderContext& rc)
	{
		//������������Ԃ��B
		if (!m_isEnable) return;

		//�V���h�E�}�b�v�p�̒萔�o�b�t�@���X�V�B
		m_shadowCb.CopyToVRAM(m_shadowCbEntity);
		//�����_�����O�X�e�b�v���V���h�E�}�b�v�쐬�ɕύX�B
		rc.SetRenderStep(EnRenderStep::enRenderStep_CreateDirectionalShadowMap);

		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			//�����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂ő҂B
			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
			//�e���h���[
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc);
			}
		}

		//�V���h�E�L���X�^�[�o�^���N���A�B
		ClearShadowCaster();
	}
	void CShadowMap::WaitEndRenderToShadowMap(RenderContext& rc)
	{

		//�e�̕`��I���B
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[i]);
		}
	}
	void CShadowMap::Update()
	{
		//�V���h�E�}�b�v�������B
		if (m_isEnable == false) {
			return;
		}
		//�V�[���̃����_�����O�Ɏg�p���Ă���J�������g���ă��C�g�̉�]�����߂�B
		Vector3 cameraDir = g_camera3D->GetForward();
		if (fabs(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON) {
			//�قڐ^��������Ă���B
			return;
		}
		//���C�g�r���[�s��̉�]�������v�Z����B
		Vector3 lightViewForward = m_lightDirection;
		Vector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//���C�g���قڐ^��������Ă���
			lightViewUp.Cross(lightViewForward, Vector3::Right);
		}
		else {
			lightViewUp.Cross(lightViewForward, Vector3::Up);
		}
		lightViewUp.Normalize();	//���K���B
		Vector3 lightViewRight;		//���C�g�̉E�����B
		lightViewRight.Cross(lightViewUp, lightViewForward);
		lightViewRight.Normalize();

		Matrix lightViewRot;	//���C�g�r���[�̌��������߂�B
		//���C�g�r���[�̉���ݒ�B
		lightViewRot.m[0][0] = lightViewRight.x;
		lightViewRot.m[0][1] = lightViewRight.y;
		lightViewRot.m[0][2] = lightViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//���C�g�r���[�̏��ݒ�B
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//���C�g�r���[�̑O��ݒ�B
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;

		//���C�g�r���[�̍������v�Z�B����悤�ɂ���B
		float lightHeight = m_lightHeight;

		float nearPlaneZ = 0.0f;	//�ߕ��ʁB
		float farPlaneZ;			//�����ʁB
		Vector3 cameraUp;			//�J�����̏����
		cameraUp.Cross(g_camera3D->GetRight(), g_camera3D->GetForward());
		//�J�X�P�[�h�V���h�E�̂��߂̏����B
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			farPlaneZ = nearPlaneZ + m_shadowAreas[i];		//�ߕ���+�V���h�E�͈̔́B
			Matrix mLightView = Matrix::Identity;			//���C�g�r���[�B
			float halfViewAngle = g_camera3D->GetViewAngle() * 0.5f;		//��p�̔����B
			//�������8���_�����C�g��Ԃɕϊ�����AABB�����߂āA���ˉe�̕��A���������߂�B
			float w, h;						//����,��
			float far_z = -1.0f;			//������̉��s�B
			Vector3 v[8];					//������̒��_
			{
				float t = tan(halfViewAngle);
				Vector3 toUpperNear, toUpperFar;			//������̋ߕ��ʁA�����ʂ̏�����x�N�g���B
				toUpperNear = cameraUp * t * nearPlaneZ;
				toUpperFar = cameraUp * t * farPlaneZ;
				t *= g_camera3D->GetAspect();		//�A�X�y�N�g��������Ă���H
				//�ߕ��ʂ̒������W���v�Z�B
				Vector3 nearPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * nearPlaneZ;
				//�ߕ��ʂ̒������W + (�J�����̉E�P�ʃx�N�g�� * �c���̔䗦(tan) * �ߕ��ʂ܂ł̋����B) + �ߕ��ʂ̏�����̃x�N�g���B
				v[0] = nearPlaneCenterPos + g_camera3D->GetRight() * t * nearPlaneZ + toUpperNear;		//�ߕ��ʂ̉E��B
				v[1] = v[0] - toUpperNear * 2.0f;														//�ߕ��ʂ̉E���B

				v[2] = nearPlaneCenterPos + g_camera3D->GetRight() * -t * nearPlaneZ + toUpperNear;		//�ߕ��ʂ̍���B
				v[3] = v[2] - toUpperNear * 2.0f;														//�ߕ��ʂ̍����B

				//�����ʂ̒������W���v�Z�B
				Vector3 farPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * farPlaneZ;

				v[4] = farPlaneCenterPos + g_camera3D->GetRight() * t * farPlaneZ + toUpperFar;			//�����ʂ̉E��B
				v[5] = v[4] - toUpperFar * 2.0f;														//�����ʂ̉E���B

				v[6] = farPlaneCenterPos + g_camera3D->GetRight() * -t * farPlaneZ + toUpperFar;		//�����ʂ̍���B
				v[7] = v[6] - toUpperFar * 2.0f;														//�����ʂ̍����B

				//���C�g�s����쐬�B
				Vector3 viewFrustumCenterPosition = (nearPlaneCenterPos + farPlaneCenterPos) * 0.5f;	//�ߕ��ʂƉ����ʂ̒������W�̊ԁB
				Vector3 lightPos = CalcLightPosition(lightHeight, viewFrustumCenterPosition);			//�J�����̈ʒu���v�Z�B

				mLightView = lightViewRot;

				mLightView.m[3][0] = lightPos.x;		//���C�g�̍��W�𕽍s�ړ������ɑ���B
				mLightView.m[3][1] = lightPos.y;		//
				mLightView.m[3][2] = lightPos.z;		//
				mLightView.m[3][3] = 1.0f;
				mLightView.Inverse(mLightView);			//���C�g�r���[�����B
				//��������\������8���_�����C�g��Ԃɍ��W�ϊ����āAAABB�����߂�B
				Vector3 vMax = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };
				Vector3 vMin = { FLT_MAX ,FLT_MAX ,FLT_MAX };
				for (auto& vInLight : v) {
					mLightView.Apply(vInLight);
					vMax.Max(vInLight);
					vMin.Min(vInLight);
				}
				w = vMax.x - vMin.x;
				h = vMax.y - vMin.y;
				far_z = vMax.z;
				//������̒��_��S�Ďʂ��v���W�F�N�V�����s������B
				Matrix proj;
				proj.MakeOrthoProjectionMatrix(
					w,
					h,
					far_z / 100.0f,
					far_z
				);
				m_LVPMatrix[i] = mLightView * proj;									//���C�g�r���[�v���W�F�N�V�����s����쐬�B
				m_shadowCbEntity.mLVP[i] = m_LVPMatrix[i];							//�萔�o�b�t�@�p�ɋL�^�B
				m_shadowCbEntity.shadowAreaDepthInViewSpace[i] = farPlaneZ * 0.8f;	//���E���ӂ����肭�`�悷�邽�߂ɁA�G���A���������߂�H
				nearPlaneZ = farPlaneZ;
			}
		}

	}
	Vector3 CShadowMap::CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition)
	{
		//�������ꂽ��������ʂ����߂̃��C�g�������v�Z����B
		float alpha = (lightHeight - viewFrustomCenterPosition.y) / m_lightDirection.y;
		Vector3 lightPos = viewFrustomCenterPosition + m_lightDirection * alpha;
		return lightPos;
	}
}