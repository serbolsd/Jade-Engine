/*****************************************************************************/
/**
	* @file    jdRay.h
	* @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
	* @date    14/JUN/2020
	* @brief   A basic quaternion
	*
	* The quaternion had component x, y, z, w
	*
	*
	* @bug	    No known bugs.
	*/
	/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"

namespace jdEngineSDK {
		class Quaternion {
   public:
    /**
     * @brief Default constructor (no initialization).
     */
    FORCEINLINE Quaternion() = default;

    /**
     * @brief Constructor.
     * @param InX x component of the quaternion
     * @param InY y component of the quaternion
     * @param InZ z component of the quaternion
     * @param InW w component of the quaternion
     */
    FORCEINLINE Quaternion(float x, float y, float z, float w);
   public:
    /**
     * @brief The quaternion's X-component.
     */
    float x;

    /**
     * @brief The quaternion's Y-component.
     */
    float y;

    /**
     * @brief The quaternion's Z-component.
     */
    float z;

    /**
     * @brief The quaternion's W-component.
     */
    float w;
		};
}
