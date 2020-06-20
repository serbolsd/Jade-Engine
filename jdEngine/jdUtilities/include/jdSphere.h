/*****************************************************************************/
/**
 * @file    jdSphere.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    13/JUN/2020
 * @brief   A basic Spehere
 *
 * The Sphere had center and radius
 *
 *
 * @bug	    No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"

namespace jdEngineSDK {
	 class JD_UTILITY_EXPORT Sphere	{
/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /**
     * @brief Default Creates and initializes a new sphere with the specified parameters.
     * @param center is the Center of sphere.
     * @param radius is the Radius of sphere.
     */
    Sphere(const JDVector3& center = {0.0f}, float radius = 0.0f) 
      : m_center(center), m_radius(radius) {}

/*****************************************************************************/
/**
 * Operators
 */
/*****************************************************************************/
   public:
    /**
     * @brief compare this sphere with other.
     * @param other the esphere to comparate.
     * @retur true if their volume are equals.
     */
    bool
    operator==(const Sphere& other) const;
    
    /**
     * @brief compare this sphere with other.
     * @param other the esphere to comparate.
     * @retur true if their volume are diferents.
     */
    bool
    operator!=(const Sphere& other) const;

/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/
   public:
    /**
     * @brief Get volume of the current sphere
     * @return Volume (in Engine units).
     */
    float
    getVolume() const;
    
    /**
     * @brief Set center of the sphere
     * @param center is the vector with the new center.
     */
    void
    setCenter(const JDVector3& center);
    
    /**
     * @brief Get center of the sphere
     * @return the current center of the sphere.
     */
    FORCEINLINE JDVector3
    getCenter() const {
    		return m_center;
    };
    
    /**
     * @brief Set center of the sphere
     * @param radius for the spehere.
     */
    void
    setRadius(const float& radius);
    
    /**
     * @brief Get redius of the sphere
     * @return the current redius of the sphere.
     */
    FORCEINLINE float
    getRadius() const {
    		return m_radius;
    };

/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
   private:
    /**
     * @brief The sphere's center point.
     */
    JDVector3 m_center;
    
    /**
     * @brief The sphere's radius.
     */
    float m_radius;
  };
}

