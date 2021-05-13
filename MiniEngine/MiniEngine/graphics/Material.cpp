#include "stdafx.h"
#include "Material.h"
namespace Engine {
	//ルートシグネチャとパイプラインステート周りはカリカリカリ
	enum {
		enDescriptorHeap_CB,
		enDescriptorHeap_SRV,
		enNumDescriptorHeap
	};

	void IMaterial::InitTexture(const TkmFile::SMaterial& tkmMat)
	{
		if (tkmMat.albedoMap != nullptr) {
			m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize);
		}
		if (tkmMat.normalMap != nullptr) {
			m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize);
		}
		if (tkmMat.specularMap != nullptr) {
			m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize);
		}
		//TODO:リフレクションマップと屈折率マップの初期化。
	}
	void IMaterial::InitFromTkmMaterila(
		const TkmFile::SMaterial& tkmMat,
		const SShaderInitData& shaderData)
	{
		//テクスチャをロード。
		InitTexture(tkmMat);

		//定数バッファを作成。
		SMaterialParam matParam;
		matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
		matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
		m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);
	}
	void NonSkinMaterial::BeginRender(RenderContext& rc, int maxInstance)
	{
		rc.SetRootSignature(CPipelineStatesDefault::m_modelDrawRootSignature);

		const auto renderStep = rc.GetRenderStep();

		switch (renderStep)
		{
		case enRenderStep_Undef:
			break;
		case enRenderStep_CreateDirectionalShadowMap:
			//シャドウマップの描画。
			if (maxInstance > 1) {
				rc.SetPipelineState(CPipelineStatesDefault::m_ModelShadowInstancingPipelineState);
			}
			else {
				rc.SetPipelineState(CPipelineStatesDefault::m_ModelShadowPipelineState);
			}
			break;
		case enRenderStep_PreRender:
			//G-Bufferの描画。
			if (maxInstance > 1) {
				rc.SetPipelineState(CPipelineStatesDefault::m_ModelInstancingPipelineState);
			}
			else {
				rc.SetPipelineState(CPipelineStatesDefault::m_ModelPipelineState);
			}

			break;
		case enRenderStep_ForwardRender:
			//半透明モデルの描画。
			rc.SetPipelineState(CPipelineStatesDefault::m_transModelPipelineState);

			break;
		default:
			break;
		}
	}
	void SkinMaterial::BeginRender(RenderContext& rc, int maxInstance)
	{
		rc.SetRootSignature(CPipelineStatesDefault::m_modelDrawRootSignature);

		const auto renderStep = rc.GetRenderStep();

		switch (renderStep)
		{
		case enRenderStep_Undef:
			break;
		case enRenderStep_CreateDirectionalShadowMap:
			//シャドウマップの描画。
			if (maxInstance > 1) {
				rc.SetPipelineState(CPipelineStatesDefault::m_SkinModelShadowInstancingPipelineState);
			}
			else {
				rc.SetPipelineState(CPipelineStatesDefault::m_SkinModelShadowPipelineState);
			}
			break;
		case enRenderStep_PreRender:
			//G-Bufferの描画。
			if (maxInstance > 1) {
				rc.SetPipelineState(CPipelineStatesDefault::m_SkinModelInstancingPipelineState);
			}
			else {
				rc.SetPipelineState(CPipelineStatesDefault::m_SkinModelPipelineState);
			}

			break;
		case enRenderStep_ForwardRender:
			//半透明モデルの描画。
			rc.SetPipelineState(CPipelineStatesDefault::m_transSkinModelPipelineState);

			break;
		default:
			break;
		}
	}
}