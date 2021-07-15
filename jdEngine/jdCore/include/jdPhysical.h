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
#include <PxFoundation.h>
#include <jdModulo.h>
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <PxPhysicsVersion.h>
#include <PxScene.h>
#include <PxFiltering.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <pvd/PxPvd.h>
#include <PxConfig.h>

#include "jdRigidBody.h"
using namespace physx;

namespace jdEngineSDK {
		class JD_CORE_EXPORT PhysicalManager : public Module<PhysicalManager>	{
		 public:
    /**
     * @brief Default constructor
     */
    PhysicalManager() {};
    
    /**
     * @brief Default destructor
     */
    virtual ~PhysicalManager() { release(); };

    /**
     * @brief Function to initialize physics system
     */
    virtual void 
    onCreate();

    /**
     * @brief Function to release members
     */
    virtual void 
    release();

    /**
     * @brief Function advance the simulation
     * @note all in it can move to simulate
     */
    virtual bool
    advanced(const float& dt);

    /**
     * @brief Function to create a static rigidBody with a sphere shape
     * @param ratio is the sphere큦 ratio
     * @param position is the body큦 position
     */
    virtual SPtr<RigidBody>
    createRigidStaticSphere(const float& ratio, JDVector3& position);

    /**
     * @brief Function to create a dynamic rigidBody with a sphere shape
     * @param ratio is the sphere큦 ratio
     * @param position is the body큦 position
     */
    virtual SPtr<RigidBody>
    createRigidDynamicSphere(const float& ratio,  JDVector3& position, const float& density);

    /**
     * @brief Function to simulate Physics
     * @param dt is the delta time
     */
    virtual void
    simulated(const float& dt);

    /**
     * @brief Function to set simulation time
     * @param time is the time to set
     */
    virtual void
    setTime(const float& time);

    /**
     * @brief Function to set gravity to scene
     * @param gravity is the gravity to set
     */
    virtual void
    setGravity(const JDVector3& gravity);

    /**
     * @brief Function to get gravity of scene
     */
    virtual JDVector3
    getGravity() const;
     
		 private:
    PxDefaultErrorCallback m_defaultErrorCallback;
    PxDefaultAllocator m_defaultAllocatorCallback;
    PxFoundation* m_pFundation = nullptr;
    PxPvd* m_pPvd = nullptr;
    PxPhysics* m_pPhysics = nullptr;
    PxCooking* m_pCooking = nullptr;
    PxScene* m_pScene = nullptr;
    PxSimulationFilterShader m_defaultFilter;
    JDVector3 m_gravity = { 0.0f, -50, 0.0f };
    float m_accumulator = 0.0f;
    float m_stepSize = 1.0f / 60.0f;
		};

  JD_CORE_EXPORT PhysicalManager&
  g_Physics();
}