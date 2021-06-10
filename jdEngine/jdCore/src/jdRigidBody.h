/*****************************************************************************/
/**
 * @file    jdPhysical.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    02/JUN/2021
 * @brief   temp class for physical using Physx
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include <PxRigidBody.h>

#include "jdVector4.h"
using namespace physx;

namespace jdEngineSDK {

  namespace RIGIDBODY_TYPE {
    enum E {
      STATIC = 0,
      DYNAMIC
    };
  }

  class RigidBody {
   public:
    RigidBody() {};
    ~RigidBody() {};
   
   private:
    RIGIDBODY_TYPE::E m_typeRigid;
    JDVector3 m_velocity;
    float m_mass;
  };
}