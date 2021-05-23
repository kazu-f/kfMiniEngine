#include "stdafx.h"
#include "Tonemap.h"

namespace Engine {
	namespace {
		const wchar_t* SHADER_FILEPATH = L"Assets/shader/tonemap.fx";

		//-----------------------------------------------------------------------------
		// Name: GetSampleOffsets_DownScale4x4
		// Desc: Get the texture coordinate offsets to be used inside the DownScale4x4
		//       pixel shader.
		//-----------------------------------------------------------------------------
		HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, Vector4 avSampleOffsets[])
		{
			if (NULL == avSampleOffsets)
				return E_INVALIDARG;

			float tU = 1.0f / dwWidth;
			float tV = 1.0f / dwHeight;

			// Sample from the 16 surrounding points. Since the center point will be in
			// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
			// center.
			int index = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					avSampleOffsets[index].x = (x - 1.5f) * tU;
					avSampleOffsets[index].y = (y - 1.5f) * tV;

					index++;
				}
			}

			return S_OK;
		}
	}

	void CTonemap::Init(const SPostEffectConfig& config)
	{
		if (!config.tonemap.isEnable)
		{
			ENGINE_LOG("�g�[���}�b�v��K�p���Ȃ��B")
			return;
		}

		//�����_�����O�^�[�Q�b�g�̍쐬�B
		CreateRenderTargets();

		//���[�g�V�O�l�`���̍쐬�B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);

		//�V�F�[�_�[�̏������B
		InitShader();

		//�p�C�v���C���X�e�[�g�̍쐬�B


		//�萔�o�b�t�@�̍쐬�B
		CreateConstantBuffer();

		//�f�B�X�N���v�^�q�[�v�̍쐬�B
		CreateDescriptorHeap();

		m_isEnable = config.tonemap.isEnable;
	}

	void CTonemap::CreateRenderTargets()
	{
	}

	void CTonemap::InitShader()
	{
		m_vsShader.LoadVS(SHADER_FILEPATH, "VSMain");
		m_psCalcLuminanceLogAverage.LoadPS(SHADER_FILEPATH, "PSCalcLuminanceLogAverage");
		m_psCalcLuminanceAverage.LoadPS(SHADER_FILEPATH, "PSCalcLuminanceAverage");
		m_psCalcLuminanceExpAverage.LoadPS(SHADER_FILEPATH, "PSCalcLuminanceExpAverage");
		m_psCalcAdaptedLuminance.LoadPS(SHADER_FILEPATH, "PSCalcAdaptedLuminance");
		m_psCalcAdaptedLuminanceFirst.LoadPS(SHADER_FILEPATH, "PSCalcAdaptedLuminanceFirst");
		m_psFinal.LoadPS(SHADER_FILEPATH, "PSFinal");
	}

	void CTonemap::CreateConstantBuffer()
	{
		m_cbTonemapCommon.Init(sizeof(m_tonemapParam), &m_tonemapParam);
		m_cbCalcLuminanceLog.Init(sizeof(m_avSampleOffsets), m_avSampleOffsets);
	}

	void CTonemap::CreateDescriptorHeap()
	{
		//���ϋP�x�v�Z�p�̃f�B�X�N���v�^�q�[�v�̍쐬�B
		{
			//�z��ԍ��B
			int curNo = NUM_CALC_AVG_RT - 1;
			//�P�x�̑ΐ����ς̌v�Z�B
			m_calcAvgDescriptorHeap[curNo].RegistConstantBuffer(0, m_cbCalcLuminanceLog);
			m_calcAvgDescriptorHeap[curNo].RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());
			m_calcAvgDescriptorHeap[curNo].Commit();

			curNo--;
			//�e�N�X�`���̃_�E���T���v�����O�B
			while (curNo > 0) {
				m_calcAvgDescriptorHeap[curNo].RegistConstantBuffer(0, m_cbCalcLuminanceLog);
				m_calcAvgDescriptorHeap[curNo].RegistShaderResource(0, m_calcAvgRT[curNo + 1].GetRenderTargetTexture());
				m_calcAvgDescriptorHeap[curNo].Commit();
				curNo--;
			}
			//�P�x�̕��ώw���̌v�Z�B
			m_calcAvgDescriptorHeap[curNo].RegistConstantBuffer(0, m_cbCalcLuminanceLog);
			m_calcAvgDescriptorHeap[curNo].RegistShaderResource(0, m_calcAvgRT[curNo + 1].GetRenderTargetTexture());
			m_calcAvgDescriptorHeap[curNo].Commit();

		}//���ϋP�x�v�Z�p�̃f�B�X�N���v�^�q�[�v�I���B

		//���Ï����̌v�Z�B
		for (int i = 0; i < 2; i++)
		{
			m_calcAdaptedLuminanceDescriptorHeap[i].RegistConstantBuffer(1, m_cbTonemapCommon);
			m_calcAdaptedLuminanceDescriptorHeap[i].RegistShaderResource(1, m_calcAvgRT[0].GetRenderTargetTexture());	//���ϋP�x�B
			m_calcAdaptedLuminanceDescriptorHeap[i].RegistShaderResource(2, m_avgRT[1 ^ i].GetRenderTargetTexture());	//��t���[���O�̕��ϋP�x�B
		}

		//�ŏI�`��p�̃f�B�X�N���v�^�q�[�v�B
		for (int i = 0; i < 2; i++) {
			m_finalDescriptorHeap[i].RegistShaderResource(0, GraphicsEngine()->GetMainRenderTarget().GetRenderTargetTexture());		//�V�[���e�N�X�`���B
			m_finalDescriptorHeap[i].RegistShaderResource(1, m_avgRT[i].GetRenderTargetTexture());
			m_finalDescriptorHeap[i].Commit();
		}
	}

}