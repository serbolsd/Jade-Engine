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
#include "jdMatrix4.h"
#include "jdRadian.h"

namespace jdEngineSDK {
  class JD_UTILITY_EXPORT Quaternion {
/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /**
     * @brief Default constructor (no initialization).
     */
    FORCEINLINE Quaternion() = default;
    
    /**
     * @brief Constructor.
     * @param x is the x component of the quaternion
     * @param y is the y component of the quaternion
     * @param z is the z component of the quaternion
     * @param w is the w component of the quaternion
     */
    Quaternion(const float& x, const float& y, const float& z);
    Quaternion(const Radian& x, const Radian& y, const Radian& z);
    Quaternion(const Degree& x, const Degree& y, const Degree& z);
    
    /**
     * @brief Constructor.
     * @param x, y, z is for x, y, z components of the quaternion
     * @param w is the w component of the quaternion
     */
    Quaternion(JDVector3 xyz, const float& w);
    
    /**
     * @brief Copy Constructor 
     * @param quaternion is the quaternion to copy
     */
    Quaternion(const Quaternion& quaternion);

/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /** 
     * @brief equals a quaternion 
     * @param quaternion is a quaternion what this with which is equaled
     * @return this quaternio equalize.
     */
    Quaternion& 
    operator=(const Quaternion& quaternion);
    
    /** 
     * @brief add two quaternions
     * @param quaternion is another quaternion.
     * @return the addition in this quaternion.
     */
    Quaternion&
    operator+=(const Quaternion& quaternion);
    
    /**
     * @brief add two quaternions
     * @param quaternion is another quaternion.
     * @return a quiaternion with the addition.
     */
    Quaternion 
    operator+(const Quaternion& quaternion) const;
    
    /**
     * @brief subtract two quaternions.
     * @param quaternion is another quaternion.
     * @return a quiaternion with the subtraction in this quaternion.
     */
    Quaternion&
    operator-=(const Quaternion& quaternion);
    
    /**
     * @brief subtract two quaternions
     * @param quaternion is another quaternion.
     * @return a quiaternion with the subtraction.
     */
    Quaternion 
    operator-(const Quaternion& quaternion) const;
    
    /** 
     * @brief Multiply two quaternion.
     * @param quaternion Another quaternion.
     * @return the multiplication in this quaternion
     */
    Quaternion&
    operator*=(const Quaternion& quaternion);
    
    /**
     * @brief Multiply two quaternion.
     * @param quaternion Another quaternion.
     * @return a quaternion with the multiplication
     */
    Quaternion 
    operator*(const Quaternion& quaternion) const;
    
    /**
     * @brief Multiply this quaternion with a scale
     * @param scale is the value to multiply the quaternion.
     * @return the multiplication in this quaternion
     */
    Quaternion&
    operator*=(const float& scale);
    
    /**
     * @brief Multiply this quaternion with a scale
     * @param scale is the value to multiply the quaternion.
     * @return a quaternion with the multiplication
     */
    Quaternion 
    operator*(const float& scale) const;
    
    /**
     * @brief Compare two quaternions
     * @param quaternion is the quaternion with which it will be compared
     * @return true if this quaternion is identical to the other  
     */
    bool
    operator==(const Quaternion& quaternion) const;
    
    /**
     * @brief Compare two quaternions
     * @param quaternion is the quaternion with which it will be compared
     * @return true if this quaternion is diferent to the other  
     */
    bool
    operator!=(const Quaternion& quaternion) const;
    
    /**
     * @brief Compare two quaternions
     * @param quaternion is the quaternion with which it will be compared
     * @return true if this quaternion is less than the other  
     */
    bool
    operator<(const Quaternion& quaternion) const;
    
    /**
     * @brief Compare two quaternions
     * @param quaternion is the quaternion with which it will be compared
     * @return true if this quaternion is greater than the other  
     */
    bool 
    operator>(const Quaternion& quaternion) const;

/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/
    /** 
     * @brief get the magnitud of this quaternion
     * @return the magnitude of this quaternion.
     */
    float
    magnitude() const;
    
    /**
     * @brief get the sauqre magnitud of this quaternion
     * @return the magnitude of this quaternion.
     */
    float
    squareMagnitude() const;
    
    /** 
     * @brief tranform this quaternion to it's normalized form.
     * @return this quaternion normalized.
     */
    Quaternion&
    normalize();
    
    /** 
     * @brief tranform this quaternion to it's normalized form.
     * @return None.
     */
    Quaternion
    getNormalize() const;
    
    /** 
     * @brief conjugate this quaternion.
     * @return this quaternion conjugated.
     */
    Quaternion&
    conjugate();
    
    /**
     * @brief get the conjugate of this quaternion.
     * @return A quaternion with the conjugate of this quaternion.
     */
    Quaternion
    getConjugate() const;
    
    /**
     * @brief invert this quaternion.
     * @return this quaternion inverted.
     */
    Quaternion& 
    inverse();
    
    /**
     * @brief get the inverse form of this quaternion.
     * @return A quaternion with the inverse form of this quaternion.
     */
    Quaternion 
    getInverse() const;

    /**
     * @brief get to rotate this quaternion.
     * @return this quaternion rotated
     */
    Quaternion&
    rotate(const float& _x, const float& _y, const float& _z, bool degree);

    /**
     * @brief get to rotate this quaternion.
     * @return this quaternion rotated
     */
    Quaternion&
    rotate(Degree _x, Degree _y, Degree _z);

    /**
     * @brief get to rotate this quaternion.
     * @return this quaternion rotated
     */
    JDVector3
    getEuler();

    JDMatrix4
    getMatrix() const;
/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
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
