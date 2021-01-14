#include "stdafx.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "Material.h"
namespace Engine {
	MeshParts::~MeshParts()
	{
		for (auto& mesh : m_meshs) {
			//インデックスバッファを削除。
			for (auto& ib : mesh->m_indexBufferArray) {
				delete ib;
			}
			//マテリアルを削除。
			for (auto& mat : mesh->m_materials) {
				delete mat;
			}
			//メッシュを削除。
			delete mesh;
		}
	}
	void MeshParts::InitFromTkmFile(
		const TkmFile& tkmFile,
		const wchar_t* fxFilePath,
		const char* vsEntryPointFunc,
		const char* psEntryPointFunc,
		void* expandData,
		int expandDataSize,
		IShaderResource* expandShaderResourceView,
		StructuredBuffer* instancingDataSB,
		int maxInstance
	)
	{
		//インスタンシング描画用のデータ。
		m_instancingDataPtr = instancingDataSB;
		m_maxInstance = maxInstance;
		
		m_meshs.resize(tkmFile.GetNumMesh());
		int meshNo = 0;
		tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
			//tkmファイルのメッシュ情報からメッシュを作成する。
			CreateMeshFromTkmMesh(mesh, meshNo, fxFilePath, vsEntryPointFunc, psEntryPointFunc);


			meshNo++;
			});
		//共通定数バッファの作成。
		m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
		//ユーザー拡張用の定数バッファを作成。
		if (expandData) {
			m_expandConstantBuffer.Init(expandDataSize, nullptr);
			m_expandData = expandData;
		}
		m_expandShaderResourceView = expandShaderResourceView;
		////ディスクリプタヒープを作成。
		//CreateDescriptorHeaps();
	}

	void MeshParts::CreateDescriptorHeaps()
	{
		//ディスクリプタヒープはマテリアルの数分だけ作成される。
		int numDescriptorHeap = 0;
		for (auto& mesh : m_meshs) {
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				numDescriptorHeap++;
			}
		}
		//ディスクリプタヒープをドカッと確保。
		m_descriptorHeap.resize(numDescriptorHeap);
		//ディスクリプタヒープを構築していく。
		int descriptorHeapNo = 0;
		for (auto& mesh : m_meshs) {
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				auto& descriptorHeap = m_descriptorHeap[descriptorHeapNo];
				//ディスクリプタヒープにディスクリプタを登録していく。
				descriptorHeap.RegistShaderResource(0, mesh->m_materials[matNo]->GetAlbedoMap());		//アルベドマップ(0番)。
				descriptorHeap.RegistShaderResource(1, mesh->m_materials[matNo]->GetNormalMap());		//法線マップ(1番)。
				descriptorHeap.RegistShaderResource(2, mesh->m_materials[matNo]->GetSpecularMap());	//スペキュラマップ(2番)。
				descriptorHeap.RegistShaderResource(3, m_boneMatricesStructureBuffer);				//ボーンの設定(3番)。
				descriptorHeap.RegistShaderResource(4, g_graphicsEngine->GetLightManager()->GetDirectionLightStructuredBuffer());	//ライトの設定(4番)。
				for (int i = 0; i < NUM_SHADOW_MAP; i++) {
					//シャドウマップ。5～7番を使う
					descriptorHeap.RegistShaderResource(5 + i, *g_graphicsEngine->GetShadowMap()->GetShadowMapTexture(i));
				}
				if (m_instancingDataPtr != nullptr) {
					descriptorHeap.RegistShaderResource(8, *m_instancingDataPtr);			//インスタンシング描画用のデータ(8番)。
				}
				if (m_expandShaderResourceView) {
					//ユーザー拡張のシェーダーリソース。(10番)
					descriptorHeap.RegistShaderResource(EXPAND_SRV_REG__START_NO, *m_expandShaderResourceView);
				}
				descriptorHeap.RegistConstantBuffer(0, m_commonConstantBuffer);											//モデルの定数バッファ(0番)。
				descriptorHeap.RegistConstantBuffer(1, g_graphicsEngine->GetLightManager()->GetLightParamConstantBuffer());		//ライトの設定(1番)。
				descriptorHeap.RegistConstantBuffer(2, mesh->m_materials[matNo]->GetConstantBuffer());				//マテリアルの定数バッファ(2番)。
				descriptorHeap.RegistConstantBuffer(3, g_graphicsEngine->GetShadowMap()->GetShadowMapConstantBuffer());		//シャドウマップの定数バッファ(3番)。
				if (m_expandConstantBuffer.IsValid()) {
					//ユーザー拡張の定数バッファ(4番)。
					descriptorHeap.RegistConstantBuffer(4, m_expandConstantBuffer);
				}
				//ディスクリプタヒープへの登録を確定させる。
				descriptorHeap.Commit();
				descriptorHeapNo++;
			}
		}
		//ディスクリプタヒープが作成された。
		m_isCreateDescriptHeap = true;
	}
	void MeshParts::CreateMeshFromTkmMesh(
		const TkmFile::SMesh& tkmMesh,
		int meshNo,
		const wchar_t* fxFilePath,
		const char* vsEntryPointFunc,
		const char* psEntryPointFunc)
	{
		//頂点バッファを作成。
		int numVertex = (int)tkmMesh.vertexBuffer.size();
		int vertexStride = sizeof(TkmFile::SVertex);
		auto mesh = new SMesh;
		mesh->skinFlags.reserve(tkmMesh.materials.size());
		mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
		mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

		auto SetSkinFlag = [&](int index) {
			if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
				//スキンがある。
				mesh->skinFlags.push_back(1);
			}
			else {
				//スキンなし。
				mesh->skinFlags.push_back(0);
			}
		};
		//インデックスバッファを作成。
		if (!tkmMesh.indexBuffer16Array.empty()) {
			//インデックスのサイズが2byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer16Array) {
				auto ib = new IndexBuffer;
				ib->Init(static_cast<int>(tkIb.indices.size()) * 2, 2);
				ib->Copy((void*)&tkIb.indices.at(0));

				//スキンがあるかどうかを設定する。
				SetSkinFlag(tkIb.indices[0]);

				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		else {
			//インデックスのサイズが4byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer32Array) {
				auto ib = new IndexBuffer;
				ib->Init(static_cast<int>(tkIb.indices.size()) * 4, 4);
				ib->Copy((void*)&tkIb.indices.at(0));

				//スキンがあるかどうかを設定する。
				SetSkinFlag(tkIb.indices[0]);

				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		//マテリアルを作成。
		mesh->m_materials.reserve(tkmMesh.materials.size());
		int matNo = 0;
		for (auto& tkmMat : tkmMesh.materials) {
			IMaterial* mat = nullptr;
			if (mesh->skinFlags[matNo]) {
				mat = new SkinMaterial;
			}
			else {
				mat = new NonSkinMaterial;
			}
			mat->InitFromTkmMaterila(tkmMat, fxFilePath, vsEntryPointFunc, psEntryPointFunc);
			mesh->m_materials.push_back(mat);
			matNo++;
		}

		m_meshs[meshNo] = mesh;

	}

	void MeshParts::BindSkeleton(Skeleton& skeleton)
	{
		m_skeleton = &skeleton;
		//構造化バッファを作成する。
		m_boneMatricesStructureBuffer.Init(
			sizeof(Matrix),
			m_skeleton->GetNumBones(),
			m_skeleton->GetBoneMatricesTopAddress()
		);
	}
	void MeshParts::Draw(
		RenderContext& rc,
		const Matrix& mWorld,
		const Matrix& mView,
		const Matrix& mProj
	)
	{
		if (m_isCreateDescriptHeap == false) {
			CreateDescriptorHeaps();
		}
#if 1

		//auto ligMgr = g_graphicsEngine->GetLightManager();

		//メッシュごとにドロー
		//プリミティブのトポロジーはトライアングルリストのみ。
		rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファを更新する。
		SConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;
		cb.isShadowReceiver = m_isShadowReceiver ? 1 : 0;

		m_commonConstantBuffer.CopyToVRAM(&cb);

		if (m_expandData) {
			m_expandConstantBuffer.CopyToVRAM(m_expandData);
		}
		if (m_boneMatricesStructureBuffer.IsInited()) {
			//ボーン行列を更新する。
			m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
		}
		int descriptorHeapNo = 0;
		for (auto& mesh : m_meshs) {
			//頂点バッファを設定。
			rc.SetVertexBuffer(mesh->m_vertexBuffer);
			//マテリアルごとにドロー。
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//このマテリアルが貼られているメッシュの描画開始。
				mesh->m_materials[matNo]->BeginRender(rc, m_maxInstance);
				//ディスクリプタヒープを登録。
				rc.SetDescriptorHeap(m_descriptorHeap.at(descriptorHeapNo));
				//インデックスバッファを設定。
				auto& ib = mesh->m_indexBufferArray[matNo];
				rc.SetIndexBuffer(*ib);

				//ドロー。
				rc.DrawIndexed(
					ib->GetCount(), 
					m_maxInstance > 1 ? m_maxInstance : 1
				);
				descriptorHeapNo++;
			}
		}
#endif
	}
}