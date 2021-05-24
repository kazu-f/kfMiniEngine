#include "stdafx.h"
#include "PostEffect.h"

namespace Engine {
	namespace {
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
	}
	CPostEffect::CPostEffect()
	{
	}
	CPostEffect::~CPostEffect()
	{
	}
	void CPostEffect::Release()
	{
	}
	void CPostEffect::Create(const SPostEffectConfig& config)
	{
		//�g�[���}�b�v���������B
		m_tonemap.Init(config);
		//�u���[�����������B
		m_bloom.Init(config);
		//�A���`�G�C���A�X���������B
		m_fxaa.Init(config);
		//�l�p�`�v���~�e�B�u���������B
		InitPrimitive();
	}
	void CPostEffect::Render(RenderContext& rc)
	{
		//���_�o�b�t�@��ݒ�B
		rc.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		rc.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		//�v���~�e�B�u�g�|���W�[��ݒ�B
		rc.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());
		//�g�[���}�b�v�B
		m_tonemap.Render(rc);
		//�u���[���B
		m_bloom.Render(rc);
		//�A���`�B
		m_fxaa.Render(rc);
	}
	void CPostEffect::InitPrimitive()
	{
		SSimpleVertex vertices[] =
		{
			{
				Vector4(-1.0f,-1.0f,0.0f,1.0f),
				Vector2(0.0f,1.0f)
			},
			{
				Vector4(1.0f,-1.0f,0.0f,1.0f),
				Vector2(1.0f,1.0f)
			},
			{
				Vector4(-1.0f,1.0f,0.0f,1.0f),
				Vector2(0.0f,0.0f)
			},
			{
				Vector4(1.0f,1.0f,0.0f,1.0f),
				Vector2(1.0f,0.0f)
			}
		};
		short indices[] = { 0,1,2,3 };
		PrimitiveInitData primitiveData;
		//���_�o�b�t�@�̏��
		primitiveData.vertexSize = sizeof(vertices);
		primitiveData.vertexStride = sizeof(SSimpleVertex);
		primitiveData.pSrcVertexBuffer = vertices;
		//�C���f�b�N�X�o�b�t�@�̏��
		primitiveData.indexSize = sizeof(indices);
		primitiveData.indexStride = sizeof(short);
		primitiveData.pSrcIndexBuffer = indices;
		//�v���~�e�B�u�g�|���W�[�B
		primitiveData.topology = enPrimitiveTopology_TriangleStrip;

		//�쐬�B
		m_quadPrimitive.Create(primitiveData);
		//���_�̐����L�^���Ă����B
		m_primitiveVertexNum = primitiveData.vertexSize / primitiveData.indexStride;
	}
}
