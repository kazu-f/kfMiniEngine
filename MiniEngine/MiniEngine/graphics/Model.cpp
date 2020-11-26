#include "stdafx.h"
#include "Model.h"

namespace Engine {
	void Model::Init(const ModelInitData& initData, int maxInstance)
	{
		//�����̃V�F�[�_�[�����[�h���鏈�������߂Ă���̂�
		//wchar_t�^�̕�����Ȃ̂ŁA�����ŕϊ����Ă����B
		wchar_t wfxFilePath[256];
		if (initData.m_fxFilePath == nullptr) {
			ASSERT("fx�t�@�C���p�X���w�肳��Ă��܂���B")
			//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
			//std::abort();
		}

		//�C���X�^���V���O�`����s���H
		if (maxInstance > 1) {
			m_maxInstance = maxInstance;
			m_instancingData.reset(new Matrix[maxInstance]);
			m_instancingDataSB.Init(sizeof(Matrix), maxInstance, m_instancingData.get());
		}

		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);

		m_tkmFile.Load(initData.m_tkmFilePath);
		m_meshParts.InitFromTkmFile(
			m_tkmFile,
			wfxFilePath,
			initData.m_vsEntryPointFunc,
			initData.m_psEntryPointFunc,
			initData.m_expandConstantBuffer,
			initData.m_expandConstantBufferSize,
			initData.m_expandShaderResoruceView,
			m_instancingDataSB.IsInited() ? &m_instancingDataSB : nullptr,
			m_maxInstance
		);

		UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	}

	void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mBias;
		//todo	if (enUpdateAxis == enFbxUpAxisZ) {
				//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
		//	}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mBias * mScale * mRot * mTrans;
	}
	void Model::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		UpdateWorldMatrix(pos, rot, scale);
		if (m_numInstance < m_maxInstance) {
			//�C���X�^���V���O�f�[�^���X�V����B
			m_instancingData[m_numInstance] = m_world;
			m_numInstance++;
		}
		else {
			DEBUG_LOG("Model::UpdateInstancingData	invalid UpdateInstancingData.");
		}
	}
	void Model::Draw(RenderContext& rc)
	{
		if (m_maxInstance > 1) {
			m_instancingDataSB.Update(m_instancingData.get());
		}
		m_meshParts.Draw(
			rc,
			m_world,
			g_camera3D->GetViewMatrix(),
			g_camera3D->GetProjectionMatrix()
		);
	}
	void Model::Draw(RenderContext& rc, Matrix mLVP)
	{
		if (m_maxInstance > 1) {
			m_instancingDataSB.Update(m_instancingData.get());
		}
		m_meshParts.Draw(
			rc,
			m_world,
			mLVP,
			Matrix::Identity
		);
	}
}