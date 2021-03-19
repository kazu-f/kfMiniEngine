#include "stdafx.h"
#include "MeshCollider.h"
#include "graphics/Model.h"

namespace Engine {
	CMeshCollider::CMeshCollider()
	{
	}
	CMeshCollider::~CMeshCollider()
	{
	}
	void CMeshCollider::CreateFromModel(const Model& model, const Matrix* offsetMatrix)
	{
		CreateFromTkmFile(model.GetTkmFile(), offsetMatrix);
	}
	void CMeshCollider::CreateFromTkmFile(const TkmFile& tkmFile, const Matrix* offsetMatrix)
	{
		Matrix mBias;
		mBias.MakeRotationX(Math::PI * -0.5f);
		if (offsetMatrix != nullptr) {
			mBias = mBias * (*offsetMatrix);
		}

		m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();
		int numMesh = 0;
		//ラムダのユニークポインタのキャプチャがよくわからなかったなぁ
		tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
			//モデルの頂点バッファから、物理エンジン用の頂点バッファを作成する。
			{
				VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
				for (auto& vb : mesh.vertexBuffer) {
					auto pos = vb.pos;
					//バイアスをかける。
					mBias.Apply(pos);
					vertexBuffer->push_back(pos);
				}

				m_vertexBufferArray.push_back(std::move(vertexBuffer));
			}

			//モデルのインデックスバッファから、物理エンジン用のインデックスバッファを作成。
			{
				IndexBufferPtr indexBuffer = std::make_unique <IndexBuffer>();
				if (!mesh.indexBuffer16Array.empty()) {
					//16bitインデックスバッファから作成。
					for (auto& ib : mesh.indexBuffer16Array) {
						for (auto index : ib.indices) {
							indexBuffer->push_back(index);
						}
					}

				}
				else {
					//32bitインデックスバッファから作成。
					for (auto& ib : mesh.indexBuffer32Array) {
						for (auto index : ib.indices) {
							indexBuffer->push_back(index);
						}
					}
				}

				m_indexBufferArray.push_back(std::move(indexBuffer));
			}

			//BulletPhysicsのインデックスメッシュを作成。
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = m_indexBufferArray.back().get();
			VertexBuffer* vb = m_vertexBufferArray.back().get();
			indexedMesh.m_numTriangles = static_cast<int>(ib->size() / 3);
			indexedMesh.m_triangleIndexBase = reinterpret_cast<unsigned char*>(&ib->front());
			indexedMesh.m_triangleIndexStride = 12;		//12固定でいい？
			indexedMesh.m_numVertices = static_cast<int>(vb->size());
			indexedMesh.m_vertexBase = reinterpret_cast<unsigned char*>(&vb->front());
			indexedMesh.m_vertexStride = sizeof(Vector3);
			//インデックスメッシュを追加。
			m_stridingMeshInterface->addIndexedMesh(indexedMesh);

			numMesh++;
		});
		//インデックスメッシュを使ってメッシュを作り出す。
		m_meshShape = std::make_unique<btBvhTriangleMeshShape>(m_stridingMeshInterface.get(), true);
	}
}
