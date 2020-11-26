/// <summary>
/// ���b�V���p�[�c�N���X�B
/// </summary>

#pragma once

#include "tkFile/TkmFile.h"
#include "graphics/GPUBuffer/StructuredBuffer.h"
namespace Engine {
	class RenderContext;
	class Skeleton;
	class Material;
	class IShaderResource;


	/// <summary>
	/// ���b�V��
	/// </summary>
	struct SMesh {
		VertexBuffer m_vertexBuffer;						//���_�o�b�t�@�B
		std::vector< IndexBuffer* >		m_indexBufferArray;	//�C���f�b�N�X�o�b�t�@�B
		std::vector< Material* >		m_materials;			//�}�e���A���B
		std::vector<int>				skinFlags;				//�X�L���������Ă��邩�ǂ����̃t���O�B
	};

	/// <summary>
	/// ���b�V���p�[�c�B
	/// </summary>
	class MeshParts {
	public:
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~MeshParts();
		/// <summary>
		/// tkm�t�@�C�����珉����
		/// </summary>
		/// <param name="tkmFile">tkm�t�@�C���B</param>
		/// /// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
		/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
		/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
		void InitFromTkmFile(
			const TkmFile& tkmFile,
			const wchar_t* fxFilePath,
			const char* vsEntryPointFunc,
			const char* psEntryPointFunc,
			void* expandData,
			int expandDataSize,
			IShaderResource* expandShaderResourceView,
			StructuredBuffer* instancingDataSB = nullptr,
			int maxInstance = 1
		);
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mWorld">���[���h�s��</param>
		/// <param name="mView">�r���[�s��</param>
		/// <param name="mProj">�v���W�F�N�V�����s��</param>
		/// <param name="light">���C�g</param>
		void Draw(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj);
		/// <summary>
		/// �X�P���g�����֘A�t����B
		/// </summary>
		/// <param name="skeleton">�X�P���g��</param>
		void BindSkeleton(Skeleton& skeleton);
		/// <summary>
		/// �V���h�E���V�[�o�[���ǂ����B
		/// </summary>
		void SetShadowReceiverFlag(bool flag)
		{
			m_isShadowReceiver = flag;
		}
	private:
		/// <summary>
		/// tkm���b�V�����烁�b�V�����쐬�B
		/// </summary>
		/// <param name="mesh">���b�V��</param>
		/// <param name="meshNo">���b�V���ԍ�</param>
		/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
		/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
		/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
		void CreateMeshFromTkmMesh(
			const TkmFile::SMesh& mesh,
			int meshNo,
			const wchar_t* fxFilePath,
			const char* vsEntryPointFunc,
			const char* psEntryPointFunc);

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v���쐬�B
		/// </summary>
		void CreateDescriptorHeaps();
	private:
		//�g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ��B
		const int EXPAND_SRV_REG__START_NO = 10;
		/// <summary>
		/// �萔�o�b�t�@�B
		/// </summary>
		/// <remarks>
		/// ���̍\���̂�ύX������ASimpleModel.fx��CB���ύX����悤�ɁB
		/// </remarks>
		struct SConstantBuffer {
			Matrix mWorld;			//���[���h�s��B
			Matrix mView;			//�r���[�s��B
			Matrix mProj;			//�v���W�F�N�V�����s��B
			int isShadowReceiver = 0;	//�V���h�E���V�[�o�[�t���O�B
		};
		ConstantBuffer m_commonConstantBuffer;					//���b�V�����ʂ̒萔�o�b�t�@�B
		ConstantBuffer m_expandConstantBuffer;					//���[�U�[�g���p�̒萔�o�b�t�@
		IShaderResource* m_expandShaderResourceView = nullptr;	//���[�U�[�g���V�F�[�_�[���\�[�X�r���[�B
		StructuredBuffer m_boneMatricesStructureBuffer;			//�{�[���s��̍\�����o�b�t�@�B
		std::vector< SMesh* > m_meshs;							//���b�V���B
		std::vector< DescriptorHeap > m_descriptorHeap;			//�f�B�X�N���v�^�q�[�v�B
		Skeleton* m_skeleton = nullptr;							//�X�P���g���B
		StructuredBuffer* m_instancingDataPtr = nullptr;		//�C���X�^���V���O�p�`��̃f�[�^���i�[���ꂽSB�̃|�C���^�B
		int m_maxInstance = 1;									//�C���X�^���X�̍ő吔�B
		void* m_expandData = nullptr;							//���[�U�[�g���f�[�^�B
		bool m_isCreateDescriptHeap = false;					//�f�B�X�N���v�^�q�[�v���쐬����Ă��邩�B
		bool m_isShadowReceiver = false;						//�V���h�E���V�[�o�[�t���O�B
	};
}