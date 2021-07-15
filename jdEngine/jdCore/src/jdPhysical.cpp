#include "jdPhysical.h"

namespace jdEngineSDK {
  PxFilterFlags myFilter(PxFilterObjectAttributes attributes0, 
    PxFilterData filterData0, 
    PxFilterObjectAttributes attributes1,
    PxFilterData filterData1,
    PxPairFlags& pairFlags,
    const void* constantBlock,
    PxU32 constantBlockSize) {
    return PxFilterFlag::Enum::eDEFAULT;
  }

  void 
  PhysicalManager::onCreate() {

    m_pFundation = PxCreateFoundation(PX_PHYSICS_VERSION,
                                     m_defaultAllocatorCallback,
                                     m_defaultErrorCallback);
    if (!m_pFundation) {
      std::cout << ("CreateFoundation failed!") << std::endl;
      return;
    }

    //m_pPvd = PxCreatePvd(*m_pFundation);
    //PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("LOCAL_IP", 5425, 10);
    //m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    PxTolerancesScale scale;
    scale.length = 1.0f;
    scale.speed = 1;//to check

    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFundation, scale);
    if (!m_pPhysics) {
      std::cout << ("CreatePhysics failed!") << std::endl;
      return;
    }
    
    PxSceneDesc scenedesc(m_pPhysics->getTolerancesScale());
    
    scenedesc.gravity = PxVec3(0.0f, -50, 0.0f);
    m_defaultFilter = myFilter;

    PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    if (!scenedesc.cpuDispatcher)
    {
      if (!mCpuDispatcher)
        std::cout << ("PxDefaultCpuDispatcherCreate failed!") << std::endl;
      scenedesc.cpuDispatcher = mCpuDispatcher;
    }
    if (!scenedesc.filterShader)
      scenedesc.filterShader = &PxDefaultSimulationFilterShader;
    scenedesc.flags |= PxSceneFlag::eENABLE_PCM;
    scenedesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
    scenedesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
    scenedesc.sceneQueryUpdateMode = PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
    
    scenedesc.gpuMaxNumPartitions = 8;
    m_pScene = m_pPhysics->createScene(scenedesc);
    if (!m_pScene) {
      std::cout << ("CreateScene failed!") << std::endl;
      return;
    }
    //m_cooking = PxCreateCooking(PX_PHYSICS_VERSION,
    //            *m_fundation, 
    //            PxCookingParams(PxTolerancesScale()));
    //
    //if (!PxInitExtensions(*m_physics, m_pvd)) {
    //  std::cout << ("Init extensions failed!") << std::endl;
    //  return;
    //}
  }

  void PhysicalManager::release() {
    if (m_pFundation)
      m_pFundation->release();
    if (m_pPvd)
      m_pPvd->release();
    if (m_pPhysics)
      m_pPhysics->release();
    if (m_pCooking)
      m_pCooking->release();
  }

  bool 
  PhysicalManager::advanced(const float& dt) {
    if (nullptr == m_pScene) {
      return false;
    }
    m_accumulator += dt;
    if (m_accumulator < m_stepSize)
      return false;

    m_accumulator -= m_stepSize;
    m_pScene->simulate(m_stepSize);
    m_pScene->fetchResults();
    return true;
  }

  SPtr<RigidBody> 
  PhysicalManager::createRigidStaticSphere(const float& ratio, JDVector3& position) {
    RigidBody* sphere = new RigidBody;
    PxMaterial* mMaterial;

    mMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution
    if (!mMaterial)
      std::cout << "createMaterial failed!" << std::endl;
    sphere->m_pRigid = PxCreateStatic(*m_pPhysics, 
                                      PxTransform(position.x, position.y, position.z), 
                                      PxSphereGeometry(ratio), 
                                      *mMaterial);
    m_pScene->addActor(*sphere->m_pRigid);

    sphere->m_typeRigid = RIGIDBODY_TYPE::STATIC;
    SPtr<RigidBody> out(sphere);
    return out;
  }

  SPtr<RigidBody> 
  PhysicalManager::createRigidDynamicSphere(const float& ratio, 
                                            JDVector3& position, 
                                            const float& density) {
    RigidBody* sphere = new RigidBody;
    PxMaterial* mMaterial;

    mMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution
    if (!mMaterial)
      std::cout << "createMaterial failed!" << std::endl;
    PxRigidDynamic* rig = PxCreateDynamic(*m_pPhysics,
                                          PxTransform(position.x, position.y, position.z),
                                          PxSphereGeometry(ratio),
                                       *mMaterial, 
                                       0.2f);

    sphere->m_pRigid = rig;
    m_pScene->addActor(*sphere->m_pRigid);
    sphere->m_typeRigid = RIGIDBODY_TYPE::DYNAMIC;
    SPtr<RigidBody> out(sphere);
    return out;
  }

  void 
  PhysicalManager::setGravity(const JDVector3& gravity) {
    if (nullptr == m_pScene) {
      return;
    }
    m_gravity = gravity;
    m_pScene->setGravity(PxVec3(gravity.x, gravity.y, gravity.z));
  }

  JDVector3 
  PhysicalManager::getGravity() const {
    if (nullptr == m_pScene) {
      return JDVector3(0, 0, 0);
    }
    return m_gravity;
  }

  void PhysicalManager::simulated(const float& dt) {
    if (nullptr == m_pPhysics)
      return;
    advanced(dt);
  }

  void 
  PhysicalManager::setTime(const float& time) {
    m_accumulator = time;
    simulated(0.0f);
  }

  JD_CORE_EXPORT PhysicalManager& 
  g_Physics() {
    return PhysicalManager::instance();
  }
}