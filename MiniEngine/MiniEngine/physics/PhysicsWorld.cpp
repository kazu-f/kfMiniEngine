#include "stdafx.h"
#include "PhysicsWorld.h"

namespace Engine {
	namespace {
		struct MyContactResultCallback :public btCollisionWorld::ContactResultCallback {
			using ContantTaskCallback = std::function<void(const btCollisionObject& contactCollisionObject)>;
			ContantTaskCallback m_cb;
			btCollisionObject* m_me = nullptr;
			virtual btScalar	addSingleResult(
				btManifoldPoint& cp, 
				const btCollisionObjectWrapper* colObj0wrap, 
				int partId0, int index0, 
				const btCollisionObjectWrapper* colObj1Wrap, 
				int partId1, int index1)override
			{
				if (m_me == colObj0wrap->getCollisionObject()) {
					m_cb(*colObj1Wrap->getCollisionObject());
				}
				return 0.0f;
			}
		};
	}

	CPhysicsWorld::~CPhysicsWorld()
	{
		Release();
	}
	void CPhysicsWorld::Release()
	{
		m_dynamicWorld.reset();
		m_constraintSolver.reset();
		m_overlappingPairCache.reset();
		m_collisionDispatcher.reset();
		m_collisionConfig.reset();
	}
	void CPhysicsWorld::Init()
	{
		Release();
		//•¨—ƒGƒ“ƒWƒ“‚Ì‰Šú‰»B
		m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		m_collisionDispatcher = std::make_unique<btCollisionDispatcher>();
		m_overlappingPairCache = std::make_unique<btDbvtBroadphase>();
		m_constraintSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

		m_dynamicWorld = std::make_unique<btDiscreteDynamicsWorld>(
			m_collisionDispatcher.get(),
			m_overlappingPairCache.get(),
			m_constraintSolver.get(),
			m_collisionConfig.get()
			);

		m_dynamicWorld->setGravity(btVector3(0, -10, 0));
	}
	void CPhysicsWorld::Update(const float time)
	{
		m_dynamicWorld->stepSimulation(time);
	}
}
