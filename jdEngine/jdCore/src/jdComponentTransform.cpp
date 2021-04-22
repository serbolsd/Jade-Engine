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
    if (scale.x==0) {
      scale.x = 0.00001f;
    }
    if (scale.y == 0) {
      scale.y = 0.00001f;
    }
    if (scale.z == 0) {
      scale.z = 0.00001f;
    }
    JDMatrix4 matT = { scale.x, 0, 0, 0,
                       0,scale.y,0,0,
                       0,0,scale.z,0,
                       0,0,0,1 };
    JDMatrix4 traslate = { 1, 0, 0, position.x,
                           0, 1, 0, position.y,
                           0, 0, 1, position.z,
                           0, 0, 0, 1 };
    //matT *= traslate;
    //if (nullptr != m_myObject->m_parent) {
    //  if (nullptr != m_myObject->m_parent->m_parent)
    //  {
    //    matT.rotateXstayPos(Degree(rotation.x));
    //    matT.rotateYstayPos(Degree(rotation.y));
    //    matT.rotateZstayPos(Degree(rotation.z));
    //  }
    //  else
    //  {
    //    matT.rotateXstayPos(Degree(rotation.x));
    //    matT.rotateYstayPos(Degree(rotation.y));
    //    matT.rotateZstayPos(Degree(rotation.z));
    //  }
    //}
    //else {
    //  matT.rotateXstayPos(Degree(rotation.x));
    //  matT.rotateYstayPos(Degree(rotation.y));
    //  matT.rotateZstayPos(Degree(rotation.z));
    //}
    matT = traslate* matT;
    Quaternion rotaNormalize = rotation;
    matT *= rotaNormalize.getMatrix();
    //matT = matLocalTransform;
    //matT.rotateX(Degree(rotation.x));
    //matT.rotateY(Degree(rotation.y));
    //matT.rotateZ(Degree(rotation.z));
    realMatT *= matT;
    matLocalTransform = matT;
    matTransform = realMatT;
    matT.transpose();
    worldPosition = { matTransform.m_03, matTransform.m_13, matTransform.m_23 };
    right = { matT.m_00, matT.m_01, matT.m_02 };
    up = { matT.m_10, matT.m_11, matT.m_12 };
    forward = { matT.m_20, matT.m_21, matT.m_22 };
    right.normalize();
    up.normalize();
    forward.normalize();
    worldRight = { matTransform.m_00, matTransform.m_01, matTransform.m_02 };
    worldUp = { matTransform.m_10, matTransform.m_11, matTransform.m_12 };
    worldForward = { matTransform.m_20, matTransform.m_21, matTransform.m_22 };
    worldRight.normalize();
    worldUp.normalize();
    worldForward.normalize();
    matLocalTransform.transpose();
  }
  
  JDMatrix4
  CTransform::getMatrixTransform() const {
    return matTransform;
  }
}
