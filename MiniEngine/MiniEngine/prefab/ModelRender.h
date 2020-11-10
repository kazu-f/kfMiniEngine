#pragma once
/// <summary>
/// ���f�������_�[
/// </summary>

namespace Engine {
	namespace prefab {
		/// <summary>
		/// 3D���f����\������@�\�B
		/// </summary>
		class ModelRender : public IGameObject
		{
		public:
			/// <summary>
			/// �J�n����
			/// </summary>
			/// <returns></returns>
			bool Start() override;
			/// <summary>
			/// �X�V�����B
			/// </summary>
			void Update() override;
			/// <summary>
			/// �t�H���[�h�����_�����O�̃p�X�ŌĂ΂�鏈���B
			/// </summary>
			/// <param name="renderContext"></param>
			void ForwardRender(RenderContext& renderContext) override;
			/// <summary>
			/// ���f�����ړ�������B
			/// </summary>
			/// <param name="move">�ړ���</param>
			void Move(const Vector3& move)
			{
				m_position += move;
			}
			/// <summary>
			/// ���W���擾�B
			/// </summary>
			const Vector3& GetPosition() const
			{
				return m_position;
			}
			/// <summary>
			/// ���f������]������B
			/// </summary>
			/// <param name="rot">��]��</param>
			void Rotate(const Quaternion& rot)
			{
				//��]�����Z����B
				m_rotation *= rot;
			}
			/// <summary>
			/// ���W��ݒ�B
			/// </summary>
			/// <param name="pos">���W</param>
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
			}
			void SetPosition(float x, float y, float z)
			{
				m_position.Set(x, y, z);
			}
			/// <summary>
			/// ��]��ݒ�B
			/// </summary>
			/// <param name="rot"></param>
			void SetRotation(Quaternion rot)
			{
				m_rotation = rot;
			}
			/// <summary>
			/// �g�嗦��ݒ�B
			/// </summary>
			/// <param name="scale"></param>
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
			}
			void SetScale(float xScale, float yScale, float zScale)
			{
				m_scale.Set(xScale, yScale, zScale);
			}
			/// <summary>
			/// �V���h�E�L���X�^�[�t���O��ݒ�B
			/// </summary>
			/// <remarks>
			/// �V���h�E�L���X�^�[�t���O��true�ɂȂ��Ă���ƁA
			/// �V���h�E�}�b�v�ւ̃����_�����O���s����B
			/// </remarks>
			/// <param name="flag"></param>
			void SetShadowCasterFlag(bool flag)
			{
				m_isShadowCaster = flag;
			}
			/// <summary>
			/// �V���h�E���V�[�o�[�t���O��ݒ�B
			/// </summary>
			/// <remarks>
			/// �V���h�E���V�[�o�[�t���O��true�ɂȂ��Ă���ƁA
			/// �V���h�E�}�b�v���Q�Ƃ��āA���̃I�u�W�F�N�g����̉e��������B
			/// </remarks>
			/// <param name="flag"></param>
			void SetShadowReceiverFlag(bool flag)
			{
				m_isShadowReceiver = flag;
			}
			/// <summary>
			/// ���f�����擾�B
			/// </summary>
			/// <returns></returns>
			Model& GetModel()
			{
				return m_model;
			}
		private:
			/// <summary>
			/// �������X�e�[�^�X�B
			/// </summary>
			enum EnInitStatus {
				enInitStatus_NotCallInitFunc,			//�������֐����܂��B
				enInitStatus_WaitInitModel,				//���f���������҂��B
				enInitStatus_WaitInitSkeleton,			//�X�P���g���̏������҂��B
				enInitStatus_WaitInitAnimationClips,	//�A�j���[�V�����N���b�v�̏������҂��B
				enInitStatus_Completed,					//�����������B
			};
			EnInitStatus m_initStatus = enInitStatus_NotCallInitFunc;	//�������X�e�[�^�X�B
			Model m_model;		//���f���B
			Vector3 m_position = Vector3::Zero;				//���W�B
			Quaternion	m_rotation = Quaternion::Identity;	//��]�B
			Vector3 m_scale = Vector3::One;					//�g�嗦�B
			Skeleton m_skeleton;							//�X�P���g���B
			std::string m_tkmFilePath;						//tkm�t�@�C���̃t�@�C���p�X�B
			bool m_isShadowCaster = false;					//�V���h�E�L���X�^�\�t���O�B
			bool m_isShadowReceiver = false;				//�V���h�E���V�[�o�[�t���O�B
		};
	}
}


