/*****************************************************************************/
/**
 * @file    jdPoint.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/JUN/2020
 * @brief   Structure for integer points in 2-d space.
 *
 * Structure for integer points in 2-d space.
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"

#include <cmath>
#include <iostream>

namespace jdEngineSDK {

  class JDPoint {

/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /**
     * @brief Default constructor (no initialization).
     */
    JDPoint() = default;

    /**
     * @brief Create and initialize a new instance with the specified coordinates.
     * @param InX The x-coordinate.
     * @param InY The y-coordinate.
     */
    JDPoint(int32 _x, int32 _y) : x(_x), y(_y) {};

/*****************************************************************************/
/**
 * Operators
 */
/*****************************************************************************/
   public:
    /**
     * @brief Compare two points for equality.
     * @param point The other point being compared.
     * @return true if the points are equal, false otherwise.
     */
    bool
    operator==(const JDPoint& point) const;

    /**
     * @brief Compare two points for inequality.
     * @param point The other point being compared.
     * @return true if the point are not equal, false otherwise.
     */
    bool
    operator!=(const JDPoint& point) const;

    /**
     * @brief Compare two points for inequality.
     * @param point The other point being compared.
     * @return true if the point are not equal, false otherwise.
     */
    bool
    operator<(const JDPoint& point) const;

    /**
     * @brief Compare two point for inequality.
     * @param point The other point being compared.
     * @return true if the point are not equal, false otherwise.
     */
    bool
    operator>(const JDPoint& point) const;

    /**
     * @brief Add another point component-wise to this point.
     * @param point The point to add to this point.
     * @return Reference to this point after addition.
     */
    JDPoint&
    operator+=(const JDPoint& point);

    /**
     * @brief Subtract another point component-wise from this point.
     * @param point The point to subtract from this point.
     * @return Reference to this point after subtraction.
     */
    JDPoint&
    operator-=(const JDPoint& point);

    /**
     * @brief Scale this point.
     * @param Scale What to multiply the point by.
     * @return Reference to this point after multiplication.
     */
    JDPoint&
    operator*=(int32 Scale);

    /**
     * @brief Divide this point by a scalar.
     * @param Divisor What to divide the point by.
     * @return Reference to this point after division.
     */
    JDPoint&
    operator/=(int32 Divisor);

    /**
     * @brief Divide this point component-wise by another point.
     * @param point The point to divide with.
     * @return Reference to this point after division.
     */
    JDPoint&
    operator/=(const JDPoint& point);
    /**
     * @brief Multiply this point component-wise by another point.
     * @param point The point to multuply with.
     * @return Reference to this point after multiplication.
     */
    JDPoint&
    operator*=(const JDPoint& point);

    /**
     * @brief Get the result of scaling on this point.
     * @param Scale What to multiply the point by.
     * @return A new scaled point.
     */
    JDPoint
    operator*(int32 Scale) const;

    /**
     * @brief Get the result of division on this point.
     * @param Divisor What to divide the point by.
     * @return A new divided point.
     */
    JDPoint
    operator/(int32 Divisor) const;

    /**
     * @brief Get the result of addition on this point.
     * @param point The other point to add to this.
     * @return A new combined point.
     */
    JDPoint
    operator+(const JDPoint& point) const;

    /**
     * @brief Get the result of subtraction from this point.
     * @param point The other point to subtract from this.
     * @return A new subtracted point.
     */
    JDPoint
    operator-(const JDPoint& point) const;

    /**
     * @brief Get the result of division on this point.
     * @param point The other point to divide from this.
     * @return A new division point.
     */
    JDPoint
    operator/(const JDPoint& point) const;

    /**
     * @brief Positive operator
     * @return A reference to the real value of the point
     */
    const JDPoint&
    operator+() const;

    /**
     * @brief Negative operator
     * @return A new point negative of this one
     */
    JDPoint
    operator-() const;

    /**
     * @brief Equal this point to another
     * @param point is the other point to equalize.
     * @return the values of the another point in this point
     */
    FORCEINLINE JDPoint& 
    operator=(const JDPoint& point);
    

    /**
     * @brief Gets specific component of the vector.
     * @param Index the index of vector component
     * @return a const to component in index.
     */
    FORCEINLINE int32
    operator[](uint32 index) const;

    /**
     * @brief Gets specific component of the vector.
     * @param Index the index of vector component
     * @return a reference of component in index.
     */
    FORCEINLINE int32&
    operator[](uint32 index);

/*****************************************************************************/
/**
 * Friends
 */
/*****************************************************************************/

    /**
     * @brief multiplicate the components of the point for a constant
     * @param scale multply the components
     * @return the result of multiplication.
     */
    FORCEINLINE friend JDPoint
    operator*(const int32& a, const JDPoint vec);

    /**
     * @brief out stream the vector
     * @return the values of the vector.
     */
    FORCEINLINE friend std::ostream&
    operator<<(std::ostream& os, const JDPoint& vec);

/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/
    /**
     * @brief Calculate the dot product
     * @param point is the other point.
     * @return the values of the dot product of this point and another
     */
    FORCEINLINE int32
    dot(const JDPoint& point);

    /**
     * @brief Calculate the cross product
     * @param point is the other point.
     * @return the values of the cross product of this point and another
     */
    FORCEINLINE int32
    cross(const JDPoint& point);

/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
   public:
    /**
     * @brief Holds the point's x-coordinate.
     */
    int32 x;

    /**
    * @brief Holds the point's y-coordinate.
    */
    int32 y;
  };


  FORCEINLINE bool
  JDPoint::operator==(const JDPoint& point) const {
    return (x == point.x && y == point.y);
  }

  FORCEINLINE bool 
  JDPoint::operator!=(const JDPoint& point) const {
    return (x != point.x || y != point.y);
  }

  FORCEINLINE bool
  JDPoint::operator<(const JDPoint& point) const {
    return (x < point.x || y < point.y);
  }

  FORCEINLINE bool
  JDPoint::operator>(const JDPoint& point) const {
    return (x > point.x || y > point.y);
  }

  FORCEINLINE JDPoint& 
  JDPoint::operator+=(const JDPoint& point) {
    x += point.x;
    y += point.y;
    return *this;
  }

  FORCEINLINE JDPoint& 
  JDPoint::operator-=(const JDPoint& point) {
    x -= point.x;
    y -= point.y;
    return *this;
  }

  FORCEINLINE JDPoint& 
  JDPoint::operator*=(int32 Scale) {
    x *= Scale;
    y *= Scale;
    return *this;
  }

  FORCEINLINE JDPoint& 
  JDPoint::operator/=(int32 Divisor) {
    x /= Divisor;
    y /= Divisor;
    return *this;
  }
  
  FORCEINLINE JDPoint& 
  JDPoint::operator/=(const JDPoint& point) {
    x /= point.x;
    y /= point.y;
    return *this;
  }

  FORCEINLINE JDPoint&
  JDPoint::operator*=(const JDPoint& point) {
    x *= point.x;
    y *= point.y;
    return *this;
  }

  FORCEINLINE JDPoint 
  JDPoint::operator*(int32 Scale) const {
    JDPoint tempPoint;
    tempPoint.x = x * Scale;
    tempPoint.y = y * Scale;
    return tempPoint;
  }

  FORCEINLINE JDPoint 
  JDPoint::operator/(int32 Divisor) const {
    JDPoint tempPoint;
    tempPoint.x = x / Divisor;
    tempPoint.y = y / Divisor;
    return tempPoint;
  }
  
  FORCEINLINE JDPoint 
  JDPoint::operator+(const JDPoint& point) const {
    JDPoint tempPoint;
    tempPoint.x = x + point.x;
    tempPoint.y = y + point.y;
    return tempPoint;
  }

  FORCEINLINE JDPoint 
  JDPoint::operator-(const JDPoint& point) const {
    JDPoint tempPoint;
    tempPoint.x = x - point.x;
    tempPoint.y = y - point.y;
    return tempPoint;
  }
 
  FORCEINLINE JDPoint
    JDPoint::operator/(const JDPoint& point) const {
    JDPoint tempPoint;
    tempPoint.x = x / point.x;
    tempPoint.y = y / point.y;
    return tempPoint;
  }

  FORCEINLINE const JDPoint& 
  JDPoint::operator+() const {
    return *this;
  }

  FORCEINLINE JDPoint 
  JDPoint::operator-() const {
    return JDPoint(-x,-y);
  }
  
  FORCEINLINE JDPoint& 
  JDPoint::operator=(const JDPoint& point) {
    x = point.x;
    y = point.y;
    return *this;
  }

  FORCEINLINE int32 
  JDPoint::operator[](uint32 index) const {
    JD_ASSERT(index < 16);
    return (&x)[index];
  }

  FORCEINLINE int32& 
  JDPoint::operator[](uint32 index) {
    JD_ASSERT(index < 16);
    return (&x)[index];
  }

  FORCEINLINE int32
  JDPoint::dot(const JDPoint& point) {
    return (x * point.x + y * point.y);
  }

  FORCEINLINE int32 
  JDPoint::cross(const JDPoint& point) {
    return (x * point.y - y * point.x);
  }

  FORCEINLINE JDPoint
  operator*(const int32& a, const JDPoint vec) {
    return JDPoint(a * vec.x, a * vec.y);
  }

  FORCEINLINE std::ostream&
  operator<<(std::ostream& os, const JDPoint& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
  }

}
