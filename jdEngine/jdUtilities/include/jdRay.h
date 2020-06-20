/*****************************************************************************/
/**
 * @file    jdRay.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    13/JUN/2020
 * @brief   A basic Ray
 *
 * The box had base, height, long, position, rotation, scale
 *
 *
 * @bug	    No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"

namespace jdEngineSDK {
  class Ray
  {
/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /**
     * @brief Default constructor (no initialization).
     */
    Ray() = default;
    
    /**
     * @brief Constructor with params
     * @param origin is the vector origin of ray
     * @param direction is the vector direction (normalized) of ray
     * @param distance is the distance that the ray can go (if is 0 is infinite)
     */
    Ray(const JDVector3& origin, const JDVector3& direction, const float& distance = 0.f);

/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/
   public: 
    /**
     * @brief Set the origin of the ray
     * @param origin is the vector with the new origin.
     */
    void
    setOrigin(const JDVector3& origin) {
      m_origin = origin;
    };
    
    /**
     * @brief get the origin of the ray
     * @return the vector of ray's origin.
     */
    JDVector3
    getOrigin() const {
      return m_origin;
    };
    
    /**
     * @brief Set the direction of the ray
     * @param direction is the vector with the new direction.
     */
    void
    setDirection(const JDVector3& direction) {
      m_direction = direction;
    };
    
    /**
     * @brief get the direction of the ray
     * @return the vector of ray's direction.
     */
    JDVector3
    getDirection() const {
      return m_direction;
    };
    
    /**
     * @brief Set the distance of the ray
     * @param distance is the new distance for the ray (if is 0 is infinite).
     */
    void
    setDistance(const float& distance) {
      m_distance = distance;
    };
    
    /**
     * @brief Get the distance of the ray
     * @return the distance of the ray (if is 0 is infinite).
     */
    float
    getDistance() const {
      return m_distance;
    };

/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
   private:
    /**
     * @brief The Ray's origin.
     */
    JDVector3 m_origin;
    
    /**
     * @brief The Ray's direction.
     */
    JDVector3 m_direction;
    
    /**
     * @brief The Ray's distance.
     * @note the distance can be 0 for be infinite
     */
    float m_distance;
  };
}
