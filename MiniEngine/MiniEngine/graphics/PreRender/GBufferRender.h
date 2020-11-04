#pragma once

namespace Engine {
	//GBuffer�̒�`�B
	enum class EnGBuffer {
		enGBufferAlbed,			//�A���x�h�B
		enGBufferNormal,		//�@���B
		enGBufferTangent,		//�ږ@���B
		enGBufferWorldPos,		//���[���h���W�B
		enGBufferSpecular,		//�X�y�L�����B
		enGBufferShadow,		//�e�}�b�v�B
		enGBufferNum,			//GBuffer�̐��B
	};

	class CGBufferRender
	{
	public:
		CGBufferRender();
		~CGBufferRender();
		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// GBuffer���������ݐ�ɐݒ�B
		/// </summary>
		void BeginRender(RenderContext& rc);
		/// <summary>
		/// �������ݐ�����ɖ߂��B
		/// </summary>
		void EndRender(RenderContext& rc);
	public:		/////Get�֐���
		/// <summary>
		/// �����_�����O�^�[�Q�b�g���擾�B
		/// </summary>
		/// <param name="enGBuffer"></param>
		/// <returns></returns>
		RenderTarget& GetRenderTarget(EnGBuffer enGBuffer)
		{
			return m_GBuffer[static_cast<int>(enGBuffer)];
		}
		/// <summary>
		/// GBuffer�̃e�N�X�`�����擾�B
		/// </summary>
		/// <param name="enGBuffer"></param>
		/// <returns></returns>
		Texture& GetGBufferTexture(EnGBuffer enGBuffer)
		{
			return m_GBuffer[static_cast<int>(enGBuffer)].GetRenderTargetTexture();
		}
	private:
		RenderTarget m_GBuffer[static_cast<int>(EnGBuffer::enGBufferNum)];		//GBuffer�B
		//ConstantBuffer m_cb;				//�萔�o�b�t�@�B
	};	//////class CGBufferRender
}

