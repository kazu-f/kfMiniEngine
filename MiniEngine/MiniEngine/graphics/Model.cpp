#include "stdafx.h"
#include "Model.h"

namespace Engine {
	void Model::Init(const ModelInitData& initData, int maxInstance)
	{
		//内部のシェーダーをロードする処理が求めているのが
		//wchar_t型の文字列なので、ここで変換しておく。
		wchar_t wfxFilePath[256];
		ENGINE_ASSERT(initData.m_fxFilePath != nullptr,"fxファイルパスが指定されていません。")

		//インスタンシング描画を行う？
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
		m_modelUpAxis = initData.m_modelUpAxis;
		UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	}

	void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mBias;
		if (enModelUpAxis_Z == m_modelUpAxis) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
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
			//インスタンシングデータを更新する。
			m_instancingData[m_numInstance] = m_world;
			m_numInstance++;
		}
		else {
			ENGINE_LOG("Model::UpdateInstancingData	invalid UpdateInstancingData.");
		}
	}
	void Model::Draw(RenderContext& rc)
	{
		m_meshParts.Draw(
			rc,
			m_world,
			g_camera3D->GetViewMatrix(),
			g_camera3D->GetProjectionMatrix()
		);
	}
	void Model::Draw(RenderContext& rc, Matrix mLVP)
	{
		m_meshParts.Draw(
			rc,
			m_world,
			mLVP,
			Matrix::Identity
		);
	}
}