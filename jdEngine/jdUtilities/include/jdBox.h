/*****************************************************************************/
/**
 * @file    jdBox.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    12/JUN/2020
 * @brief   A basic box
 *
 * The box had width, height, long, position, rotation, scale
 *
 *
 * @bug	    No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"
#include "jdRadian.h"

namespace jdEngineSDK {

  class JD_UTILITY_EXPORT Box
  {
   public:
/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
    /**
     * @brief Default Constructor box given a base and height.
     * @param width is for width component
     * @param height is for heigh component
     * @param _long is for heigh component
     * @note initialice position on 0, 0, 0 cordinates
     */
    Box(const float& width = 0.0f, const float& height = 0.0f, const float& _long = 0.0f);
    /**
     * @brief Constructor box with other box
     * @param box is the box to copy data
     */
    Box(const Box& box);
  
/*****************************************************************************/
/**
 * Operators
 */
/*****************************************************************************/
   public:
    /**
     * @brief Equal this box to another
     * @param box is the other box to equalize.
     * @return the values of the another box in this box
     */
    Box&
    operator=(const Box& box);
    
    /**
     * @brief Gets the result of comparation of this box with another.
     * @param box is the other box to compare this by.
     * @return true if the area, the base and the height are equal.
     */
    FORCEINLINE bool
    operator==(const Box& box) const;
    
    /**
     * @brief Gets the result of comparation of this box with another.
     * @param box is the other box to compare this by.
     * @return true if the area, the base and the height are diferent.
     */
    FORCEINLINE bool
    operator!=(const Box& box) const;
  
/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/
    /**
     * @brief set box's position
     * @param pos vector with the cordinates x, y, z
     */
    FORCEINLINE void
    setPosition(const JDVector3& pos) {
      m_position = pos;
    }
    
    /**
     * @brief set box's position
     * @param posX for x cordinate
     * @param posY for y cordinate
     * @param posZ for z cordinate
     */
    FORCEINLINE void
    setPosition(const float& posX, const float& posY, const float& posZ) {
      m_position = { posX,posY,posZ };
    }
    
    /**
     * @brief Get box's position
     * @return a vector with the position
     */
    FORCEINLINE JDVector3
    getPosition() const {
      return m_position;
    }
    
    /**
     * @brief Set box's width
     * @param width is width value
     */
    FORCEINLINE void
    setWidth(const float& width) {
      m_width = width;
    }
    
    /**
     * @brief Get box's width
     * @return a float with the width multiply by scale
     */
    FORCEINLINE float
    getWidth() const {
      return m_width * m_scale;
    }
    
    /**
     * @brief Get box's width
     * @return a float with the width unmodified
     */
    FORCEINLINE float
    getRealWidth() const {
      return m_width;
    }
    
    /**
     * @brief Set box's height
     * @param height is height value
     */
    FORCEINLINE void
    setHeight(const float& height) {
      m_height = height;
    }
    
    /**
     * @brief Get box's Heigh
     * @return a float with the heigh multiply by scale
     */
    FORCEINLINE float
    getHeight() const {
      return m_height * m_scale;
    }
    
    /**
     * @brief Get box's Heigh
     * @return a float with the heigh unmodified
     */
    FORCEINLINE float
    getRealHeight() const {
      return m_height;
    }
    
    /**
     * @brief Set box's long
     * @param long is long value
     */
    FORCEINLINE void
    setLong(const float& _long) {
      m_length = _long;
    }
    
    /**
     * @brief Get box's long
     * @return a float with the long
     */
    FORCEINLINE float
    getLong() const {
      return m_length * m_scale;
    }
    
    /**
     * @brief Get box's Heigh
     * @return a float with the long unmodified
     */
    FORCEINLINE float
    getRealLong() const {
      return m_length;
    }
    
    /**
     * @brief Get box's volume
     * @return the volume of box
     */
    FORCEINLINE float
    getvolume() const {
      return m_height * m_width * m_length * m_scale;
    }
    
    /**
     * @brief Set box's Scale
     * @param scale is the value to scale
     */
    FORCEINLINE void
    setScale(const float& scale) {
      m_scale = scale;
    }
    
    /**
     * @brief Get rectangle's Scale
     * @return the scale of box
     */
    FORCEINLINE float
    getScale() const {
      return m_scale;
    }
    
    /**
     * @brief Set box's Rotation
     * @param angleRadian is the angle on radians
     */
    FORCEINLINE void
    setRotation(const Radian& angleRadian) {
      m_rotation = angleRadian;
    }
    
    /**
     * @brief get box's rotation angle as a radians
     * @return a flaoat with the box's rotation angle as a radians
     */
    FORCEINLINE float
    getRotationRadians() const {
      return m_rotation.valueRadians();
    }
    
    /**
     * @brief get box's rotation angle as a degrees
     * @return a flaoat with the box's rotation angle as a degrees
     */
    FORCEINLINE float
    getRotationDegrees() const {
      return m_rotation.valueDegrees();
    }
  
/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
   private:
    /**
     * @brief The box's scale.
     */
    float m_scale;
    
    /**
     * @brief The box's rotation.
     */
    Radian m_rotation;
    
    /**
     * @brief The box's center point.
     */
    JDVector3 m_position;
    
    /**
     * @brief The box's base .
     */
    float m_width;
    
    /**
     * @brief The box's height .
     */
    float m_height;
    
    /**
     * @brief The box's height .
     */
    float m_length;
  };
}
