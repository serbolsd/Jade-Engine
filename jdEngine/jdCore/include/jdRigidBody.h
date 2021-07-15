/*****************************************************************************/
/**
 * @file    jdRigidBody.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    02/JUN/2021
 * @brief   temp class for component rigidbody
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include <PxRigidBody.h>
#include <PxShape.h>
#include <PxActor.h>

#include <jdVector4.h>
#include <jdQuaternion.h>
#include "jdComponent.h"
using namespace physx;

namespace jdEngineSDK {

  namespace RIGIDBODY_TYPE {
    enum E {
      STATIC = 0,
      DYNAMIC
    };
  }

  class JD_CORE_EXPORT RigidBody : public Component {
   friend class PhysicalManager;
   friend class CTransform;
   public:
    /**
     * @brief default constructor
     */
    RigidBody() {};

    /**
     * @brief default destructor
     */
    ~RigidBody() {};
    
    /**
     * @brief funtion to upadate  the matrix
     * @param deltatime is the elpase timen between frames
     */
    void
    Update(const float& deltatime) override;
    
    /**
     * @brief funtion save original postion and rotation;
     */
    void
    saveOriginalData();

    /**
     * @brief funtion save original postion and rotation;
     */
    void
    LoadOriginalData();

    PxRigidActor* m_pRigid = nullptr;
   private:

    //to future use
    PxShape* m_pShape = nullptr;

    //to future use
    RIGIDBODY_TYPE::E m_typeRigid;

    //to future use
    JDVector3 m_velocity = { 0,0,0 };
    
    //to future use
    float m_mass = 0.0f;

    /**
     * @brief original postion
     */
    JDVector3 m_originalPosition = { 0, 0, 0 };

    /**
     * @brief rotation in degree
     */
    Quaternion m_originalRotation = { 0, 0, 0 };

    PxVec3 m_linearVelocity = { 0, 0, 0 };
    PxVec3 m_angularVelocity = { 0, 0, 0 };;
    PxReal m_wakeCounter = 0;

  };
}