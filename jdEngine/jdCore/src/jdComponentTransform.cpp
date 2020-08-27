#include "jdComponentTransform.h"

#include <jdDegree.h>

#include "jdGameObject.h"

namespace jdEngineSDK {
  void 
    CTransform::Update(const float& deltatime) {
    JDMatrix4 realMatT;
    if (nullptr != m_myObject->m_parent)
    {
      auto myparent = m_myObject->m_parent;
      auto trans =
        reinterpret_cast<CTransform*>(myparent->getComponent(COMPONENT_TYPE::TRANSFORM).get());
      
      realMatT = trans->getMatrixTransform();
    
    }
    JDMatrix4 matT = { scale.x, 0, 0, 0,
                       0,scale.y,0,0,
                       0,0,scale.z,0,
                       0,0,0,1 };
    JDMatrix4 traslate = { 1, 0, 0, position.x,
                           0, 1, 0, position.y,
                           0, 0, 1, position.z,
                           0, 0, 0, 1 };
    matT *= traslate;
    if (nullptr != m_myObject->m_parent) {
      matT.rotateXstayPos(Degree(rotation.x));
      matT.rotateYstayPos(Degree(rotation.y));
      matT.rotateZstayPos(Degree(rotation.z));
    }
    else {
      matT.rotateX(Degree(rotation.x));
      matT.rotateY(Degree(rotation.y));
      matT.rotateZ(Degree(rotation.z));
    }

    //matT.rotateX(Degree(rotation.x));
    //matT.rotateY(Degree(rotation.y));
    //matT.rotateZ(Degree(rotation.z));
    realMatT *= matT;
    matTransform = realMatT;
  }
  
  JDMatrix4
  CTransform::getMatrixTransform() const {
    return matTransform;
  }
}
