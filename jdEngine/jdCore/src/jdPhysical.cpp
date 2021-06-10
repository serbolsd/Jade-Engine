#include "jdPhysical.h"

namespace jdEngineSDK {
  void 
  PhysicalManager::onCreate() {

    m_fundation = PxCreateFoundation(PX_PHYSICS_VERSION, 
                                     m_defaultAllocatorCallback,
                                     m_defaultErrorCallback);
    if (!m_fundation) {
      std::cout << ("CreateFoundation failed!") << std::endl;
      return;
    }

    m_pvd = PxCreatePvd(*m_fundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("LOCAL_IP", 5425, 10);
    m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION,
                                *m_fundation, 
                                PxTolerancesScale(), 
                                true, 
                                m_pvd);
    if (!m_physics) {
      std::cout << ("CreatePhysics failed!") << std::endl;
      return;
    }

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION,
                *m_fundation, 
                PxCookingParams(PxTolerancesScale()));

    if (!PxInitExtensions(*m_physics, m_pvd)) {
      std::cout << ("Init extensions failed!") << std::endl;
      return;
    }
  }

  void PhysicalManager::release() {
    if (m_fundation)
      m_fundation->release();
    if (m_pvd)
      m_pvd->release();
    if (m_physics)
      m_physics->release();
    if (m_cooking)
      m_cooking->release();
  }
}