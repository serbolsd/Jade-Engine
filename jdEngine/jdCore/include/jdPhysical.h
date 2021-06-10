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
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <pvd/PxPvd.h>
#include <PxConfig.h>

using namespace physx;

namespace jdEngineSDK {
		class JD_CORE_EXPORT PhysicalManager : public Module<PhysicalManager>	{
		 public:
    PhysicalManager() {};
    virtual ~PhysicalManager() { release(); };

    virtual void 
    onCreate();

    virtual void 
    release();

    virtual void
    createRigidDynamic(void*);

    virtual void
    simulated();

		 private:
    PxDefaultErrorCallback m_defaultErrorCallback;
    PxDefaultAllocator m_defaultAllocatorCallback;
    PxFoundation* m_fundation;
    PxPvd* m_pvd;
    PxPhysics* m_physics;
    PxCooking* m_cooking;

    float gravity;
		};
}