#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
namespace Engine {
	class IShaderResource;

	/// <summary>
	/// ���f���̏������f�[�^
	/// </summary>
	struct ModelInitData {
		const char* m_tkmFilePath = nullptr;		//tkm�t�@�C���p�X�B
		const char* m_vsEntryPointFunc = "VSMain";	//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
		const char* m_psEntryPointFunc = "PSMain";	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
		const char* m_fxFilePath = nullptr;			//.fx�t�@�C���̃t�@�C���p�X�B
		void* m_expandConstantBuffer = nullptr;		//���[�U�[�g���̒萔�o�b�t�@�B
		int m_expandConstantBufferSize = 0;			//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
		IShaderResource* m_expandShaderResoruceView = nullptr;	//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
	};
	/// <summary>
	/// ���f���N���X�B
	/// </summary>
	class Model {

	public:

		/// <summary>
		/// tkm�t�@�C�����珉�����B
		/// </summary>
		/// <param name="initData">�������f�[�^</param>
		void Init(const ModelInitData& initData,int maxInstance = 1);
		/// <summary>
		/// ���[���h�s��̍X�V�B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);
		/// <summary>
		/// �C���X�^���V���O�`��p�̍s��f�[�^���X�V����B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		void UpdateInstancingData(
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		);
		/// <summary>
		/// GPU�ɃC���X�^���V���O�`��p�̃f�[�^�𑗂�B
		/// </summary>
		void SendGPUInstancingDatas()
		{
			if (m_maxInstance > 1) {
				m_instancingDataSB.Update(m_instancingData.get());
			}
		}

		/// <summary>
		/// �X�P���g�����֘A�t����B
		/// </summary>
		/// <param name="skeleton">�X�P���g��</param>
		void BindSkeleton(Skeleton& skeleton)
		{
			m_meshParts.BindSkeleton(skeleton);
		}

		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		void Draw(RenderContext& renderContext);
		/// <summary>
		/// /�V���h�E�}�b�v�p�`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mLVP">���C�g�r���[�v���W�F�N�V�����s��</param>
		void Draw(RenderContext& rc, Matrix mLVP);
		/// <summary>
		/// �������ς݂��H
		/// </summary>
		bool IsInited()
		{
			return m_tkmFile.IsLoaded();
		}
		/// <summary>
		/// ���[���h�s����擾�B
		/// </summary>
		/// <returns></returns>
		const Matrix& GetWorldMatrix() const
		{
			return m_world;
		}
		/// <summary>
		/// �V���h�E���V�[�o�[�ɐݒ肷��B
		/// </summary>
		void SetShadowReceiverFlag(bool flag)
		{
			m_meshParts.SetShadowReceiverFlag(flag);
		}

	private:

		Matrix m_world;			//���[���h�s��B
		TkmFile m_tkmFile;		//tkm�t�@�C���B
		MeshParts m_meshParts;	//���b�V���p�[�c�B
		std::unique_ptr<Matrix[]> m_instancingData;	//�C���X�^���V���O�`��p�̃f�[�^�B
		StructuredBuffer m_instancingDataSB;		//�C���X�^���V���O�`��p�̃o�b�t�@�B
		int m_maxInstance = 1;		//�C���X�^���V���O�`��̍ő吔�B
		int m_numInstance = 0;		//�C���X�^���X�̐��B
	};
}