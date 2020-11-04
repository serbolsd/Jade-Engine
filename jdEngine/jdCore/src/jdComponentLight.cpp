#include "jdComponentLight.h"
#include "jdComponentTransform.h"
#include "jdGameObject.h"

namespace jdEngineSDK {
  void 
  CLight::Update(const float& deltatime) {
    if (nullptr == m_myObject) {
      return;
    }
    m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    auto component = m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    if (nullptr == component) {

    }
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());

    if (m_direction != trans->forward ||
      m_position != trans->position) {
      m_position = trans->position;
      m_direction = trans->forward;
    }
  }
}
