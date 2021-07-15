#include "jdRigidBody.h"
#include "jdGameObject.h"
#include "jdComponentTransform.h"
#include "jdRigidBody.h"
#include <PxRigidDynamic.h>

namespace jdEngineSDK {
  void 
  RigidBody::Update(const float& deltatime) {
    auto component = m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    auto rigidPos = m_pRigid->getGlobalPose();
    trans->position.x = rigidPos.p.x;
    trans->position.y = rigidPos.p.y;
    trans->position.z = rigidPos.p.z;

    trans->rotation = Quaternion(rigidPos.q.x, rigidPos.q.y, rigidPos.q.z, rigidPos.q.w);
    trans->Update(deltatime);
  }

  void
  RigidBody::saveOriginalData() {
    auto component = m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    m_originalPosition = trans->position;
    m_originalRotation = trans->rotation;
    if (RIGIDBODY_TYPE::DYNAMIC == m_typeRigid) {
      auto rigDynamic = reinterpret_cast<PxRigidDynamic*>(m_pRigid);
      m_angularVelocity = rigDynamic->getAngularVelocity();
      m_linearVelocity = rigDynamic->getLinearVelocity();
      m_wakeCounter = rigDynamic->getWakeCounter();
    }
  }

  void 
  RigidBody::LoadOriginalData() {
    auto component = m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    trans->position = m_originalPosition;
    trans->rotation = m_originalRotation;
    PxTransform rigidPos;
    rigidPos.p.x = m_originalPosition.x;
    rigidPos.p.y = m_originalPosition.y;
    rigidPos.p.z = m_originalPosition.z;
    rigidPos.q = PxQuat(m_originalRotation.x, 
                        m_originalRotation.y, 
                        m_originalRotation.z, 
                        m_originalRotation.w);

    m_pRigid->setGlobalPose(rigidPos, true);
    if (RIGIDBODY_TYPE::DYNAMIC == m_typeRigid) {
      auto rigDynamic = reinterpret_cast<PxRigidDynamic*>(m_pRigid);
      rigDynamic->setAngularVelocity(m_angularVelocity);
      rigDynamic->setLinearVelocity(m_linearVelocity);
      rigDynamic->setWakeCounter(m_wakeCounter);
    }

    trans->Update(0.0f);
  }
}
