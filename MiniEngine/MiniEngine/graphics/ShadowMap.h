#pragma once


namespace Engine {
	/// <summary>
	/// �w�����V���h�E�}�b�v�N���X�B
	/// </summary>
	/// <remarks>
	/// �J�X�P�[�h�V���h�E����Ă݂�B
	/// </remarks>

	class CShadowMap : Noncopyable
	{
	public:
		/// <summary>
		/// �����������B
		/// </summary>
		void Init(SShadowMapConfig& cnfig);
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O���s���B
		/// </summary>
		void RenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�����҂��B
		/// </summary>
		void WaitEndRenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();
		/// <summary>
		/// �V���h�E�}�b�v��L���ɂ���B
		/// </summary>
		void SetEnable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// �V���h�E�}�b�v�𖳌��ɂ���B
		/// </summary>
		void SetDisable()
		{
			m_isEnable = false;
		}
	public:
		/// <summary>
		/// �L���X�^�[�֓o�^����B
		/// </summary>
		void RegistShadowCaster(Model* caster) {
			m_shadowCasters.push_back(caster);
		}
		/// <summary>
		/// �L���X�^�[�o�^���N���A�B
		/// </summary>
		/// <remarks>�G���W�����ŌĂяo���B</remarks>
		void ClearShadowCaster()
		{
			m_shadowCasters.clear();
		}


	private:	//�����I�ȏ����B
		/// <summary>
		/// ���C�g�̍��W���v�Z����B
		/// </summary>
		Vector3 CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition);

	private:		//�\���̂Ƃ�
		/// <summary>
		/// �V���h�E�}�b�v�p�̒萔�o�b�t�@�̏��B
		/// </summary>
		struct SShadowCb {
			Matrix mLVP[NUM_SHADOW_MAP];
			Vector4 texOffset[NUM_SHADOW_MAP];
			float depthOffset[NUM_SHADOW_MAP];
			float pading;
			float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//�J������Ԃł̉e�𗎂Ƃ��G���A�̐[�x�e�[�u���B
		};

		std::vector<Model*> m_shadowCasters;					//�V���h�E�L���X�^�[�̔z��B
		Matrix m_LVPMatrix[NUM_SHADOW_MAP];							//���C�g�r���[�v���W�F�N�V�����s��B
		SShadowCb m_shadowCbEntity;
	private:
		ConstantBuffer m_shadowCb;								//�萔�o�b�t�@�B
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];				//�V���h�E�}�b�v�̃����_�[�^�[�Q�b�g�B
		Vector3 m_lightDirection = { 0.0f,-1.0f,0.0f };			//���C�g�̕����B
		float m_shadowAreas[NUM_SHADOW_MAP] = { 0.0f };			//�e��������͈́B
		float m_lightHeight = 100.0f;							//���C�g�̍����B
		bool m_isEnable = true;									//�V���h�E�}�b�v���L�����ǂ����B
	};
}
