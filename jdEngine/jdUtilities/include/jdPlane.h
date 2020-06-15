/*****************************************************************************/
/**
 * @file    jdPoint.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/JUN/2020
 * @brief   Structure for three dimensional planes.
 *
 * Structure for three dimensional planes.
 * Stores the coefficients as Xx+Yy+Zz=W. Note that this is different from many
 * other Plane classes that use Xx+Yy+Zz+W=0.
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"
#include "jdVector4.h"
#include "jdMatrix4.h"

namespace jdEngineSDK {
  class Plane
  {
  public:
    /**
     * @brief Default constructor (no initialization).
     */
    FORCEINLINE Plane() = default;
  
    /**
     * @brief Constructor.
     * @param plane 4D vector to set up plane.
     */
    FORCEINLINE Plane(const JDVector4& plane);
  
    /**
     * @brief Constructor.
     * @param InX X-coefficient.
     * @param InY Y-coefficient.
     * @param InZ Z-coefficient.
     * @param InW W-coefficient.
     */
    FORCEINLINE Plane(const float& x, const float& y, const float& z, const float& w);
  
    /**
     * @brief Constructor.
     * @param InNormal Plane Normal Vector.
     * @param InW Plane W-coefficient.
     */
    FORCEINLINE Plane(const JDVector3& InNormal, float InW);
  
    /**
     * @brief Constructor.
     * @param InBase Base point in plane.
     * @param InNormal Plane Normal Vector.
     */
    FORCEINLINE Plane(const JDVector3& InBase, const JDVector3& InNormal);
  
    /**
     * @brief Calculates distance between plane and a point.
     * @param P The other point.
     * @return >0: point is in front of the plane, <0: behind, =0: on the plane.
     */
    FORCEINLINE float
    planeDot(const JDVector3& P) const;
  
    /**
     * @brief Get a flipped version of the plane.
     * @return A flipped version of the plane.
     */
    Plane
    flip() const;

    /**
     * @brief Calculates dot product of two planes.
     * @param plane The other plane.
     * @return The dot product.
     */
    FORCEINLINE float
    dot(const Plane& plane) const;

  
    /**
     * @brief Check if two planes are identical.
     * @param plane The other plane.
     * @return true if planes are identical, otherwise false.
     */
    bool
    operator==(const Plane& plane) const;
  
    /**
     * @brief Check if two planes are different.
     * @param plane The other plane.
     * @return true if planes are different, otherwise false.
     */
    bool
    operator!=(const Plane& plane) const;
  
    
    /**
     * @brief Gets result of adding a plane to this.
     * @param plane The other plane.
     * @return The result of adding a plane to this.
     */
    Plane
    operator+(const Plane& plane) const;
  
    /**
     * @brief Gets result of subtracting a plane from this.
     * @param plane The other plane.
     * @return The result of subtracting a plane from this.
     */
    Plane
    operator-(const Plane& plane) const;
  
    /**
     * @brief Gets result of dividing a plane.
     * @param Scale What to divide by.
     * @return The result of division.
     */
    Plane
    operator/(float scale) const;
  
    /**
     * @brief Gets result of scaling a plane.
     * @param Scale The scaling factor.
     * @return The result of scaling.
     */
    Plane
    operator*(float scale) const;
  
    /**
     * @brief Gets result of multiplying a plane with this.
     * @param plane The other plane.
     * @return The result of multiplying a plane with this.
     */
    Plane
    operator*(const Plane& plane);
  
    /**
     * @brief Add another plane to this.
     * @param plane The other plane.
     * @return Copy of plane after addition.
     */
    Plane&
    operator+=(const Plane& plane);
  
    /**
     * @brief Subtract another plane from this.
     * @param plane The other plane.
     * @return Copy of plane after subtraction.
     */
    Plane&
    operator-=(const Plane& plane);
  
    /**
     * @brief Scale this plane.
     * @param Scale The scaling factor.
     * @return Copy of plane after scaling.
     */
    Plane&
    operator*=(float scale);
  
    /**
     * @brief Multiply another plane with this.
     * @param plane The other plane.
     * @return Copy of plane after multiplication.
     */
    Plane&
    operator*=(const Plane& plane);
  
    /**
     * @brief Divide this plane.
     * @param plane What to divide by.
     * @return Copy of plane after division.
     */
    Plane&
    operator/=(float scale);
  
  public:
    /**
     * @brief The x,y,z-components.
     */
    JDVector3 m_xyz;
    /**
     * @brief The w-component.
     */
    float w;
  };
  
  FORCEINLINE Plane::Plane(const JDVector4& plane) : m_xyz(JDVector3(plane.x, plane.y, plane.z)), w(plane.w) {}

  FORCEINLINE Plane::Plane(const float& x, const float& y, const float& z, const float& w) :
                           m_xyz(JDVector3(x, y, z)), w(w) {}

  FORCEINLINE Plane::Plane(const JDVector3& InNormal, float InW) : m_xyz(InNormal), w(InW) {}

  FORCEINLINE Plane::Plane(const JDVector3& InBase, const JDVector3& InNormal) : 
                           m_xyz(InNormal), w(InBase.dot(InNormal)) {}

  FORCEINLINE float
  Plane::planeDot(const JDVector3& P) const {
    return m_xyz.x * P.x + m_xyz.y * P.y + m_xyz.z * P.z - w;
  }

  FORCEINLINE Plane 
  Plane::flip() const {
    return Plane(-m_xyz.x, -m_xyz.x, -m_xyz.x, -w);
  }

  FORCEINLINE float
  Plane::dot(const Plane& plane) const {
    return m_xyz.x * plane.m_xyz.x + m_xyz.y * plane.m_xyz.y + m_xyz.z * plane.m_xyz.z + w * plane.w;
  }

  FORCEINLINE bool 
  Plane::operator==(const Plane& plane) const {
    return (Math::abs(m_xyz.x - plane.m_xyz.x) < Math::EPSILONF) &&
           (Math::abs(m_xyz.y - plane.m_xyz.y) < Math::EPSILONF) &&
           (Math::abs(m_xyz.z - plane.m_xyz.z) < Math::EPSILONF) &&
           (Math::abs(w - w) < Math::EPSILONF);
  }

  FORCEINLINE bool 
  Plane::operator!=(const Plane& plane) const {
    return (Math::abs(m_xyz.x - plane.m_xyz.x) > Math::EPSILONF) ||
           (Math::abs(m_xyz.y - plane.m_xyz.y) > Math::EPSILONF) ||
           (Math::abs(m_xyz.z - plane.m_xyz.z) > Math::EPSILONF) ||
           (Math::abs(w - w) > Math::EPSILONF);
  }

  FORCEINLINE Plane 
  Plane::operator+(const Plane& plane) const {
    return Plane(m_xyz.x + plane.m_xyz.x, m_xyz.y + plane.m_xyz.y, m_xyz.z + plane.m_xyz.z, w + plane.w);
  }

  FORCEINLINE Plane
  Plane::operator-(const Plane& plane) const {
    return Plane(m_xyz.x - plane.m_xyz.x, m_xyz.y - plane.m_xyz.y, m_xyz.z - plane.m_xyz.z, w - plane.w);
  }

  FORCEINLINE Plane Plane::operator/(float scale) const {
    return Plane(m_xyz.x / scale, m_xyz.y / scale, m_xyz.z / scale, w / scale);
  }

  FORCEINLINE Plane 
  Plane::operator*(float scale) const {
    return Plane(m_xyz.x * scale, m_xyz.y * scale, m_xyz.z * scale, w * scale);
  }

  FORCEINLINE Plane 
  Plane::operator*(const Plane& plane) {
    return Plane(m_xyz.x * plane.m_xyz.x, m_xyz.y * plane.m_xyz.y, m_xyz.z * plane.m_xyz.z, w * plane.w);
  }

  FORCEINLINE Plane& 
  Plane::operator+=(const Plane& plane) {
    m_xyz.x += plane.m_xyz.x;
    m_xyz.y += plane.m_xyz.y;
    m_xyz.z += plane.m_xyz.z;
    w += plane.w;
    return *this;
  }

  FORCEINLINE Plane& 
  Plane::operator-=(const Plane& plane) {
    m_xyz.x -= plane.m_xyz.x;
    m_xyz.y -= plane.m_xyz.y;
    m_xyz.z -= plane.m_xyz.z;
    w -= plane.w;
    return *this;
  }

  FORCEINLINE Plane& 
  Plane::operator*=(float scale) {
    m_xyz.x *= scale;
    m_xyz.y *= scale;
    m_xyz.z *= scale;
    w *= scale;
    return *this;
  }

  FORCEINLINE Plane& 
  Plane::operator*=(const Plane& plane) {
    m_xyz.x *= plane.m_xyz.x;
    m_xyz.y *= plane.m_xyz.y;
    m_xyz.z *= plane.m_xyz.z;
    w *= plane.w;
    return *this;
  }

  FORCEINLINE Plane&
  Plane::operator/=(float scale) {
    m_xyz.x /= scale;
    m_xyz.y /= scale;
    m_xyz.z /= scale;
    w /= scale;
    return *this;
  }

}