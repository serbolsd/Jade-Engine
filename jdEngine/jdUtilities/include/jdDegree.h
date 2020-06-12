/*****************************************************************************/
/**
 * @file    jdDegree.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUN/2020
 * @brief   Wrapper class which indicates a given angle value is in Degrees.
 *
 * Degree values are interchangeable with Radian values, and conversions will
 * be done automatically between them.
 *
 * @bug	   No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"

namespace jdEngineSDK
{
  class Radian;
  class JD_UTILITY_EXPORT Degree
  {
   public:
    constexpr Degree() = default;

    constexpr Degree(const Degree & d) = default;

    constexpr explicit Degree(float d) : m_degree(d) {}

    Degree(const Radian & r);

    constexpr Degree&
    operator=(const Degree & d) = default;

    Degree&
    operator=(const float& f) {
      m_degree = f;
      return *this;
    }

    Degree&
    operator=(const Radian & r);

    FORCEINLINE float
    valueDegrees() const {
      return m_degree;
    }

    FORCEINLINE float
    valueRadians() const;

    /**
     * @brief Utility to ensure angle is between +/- 180 degrees by unwinding.
     */
    void
    unwindDegrees();

    const Degree&
    operator+() const {
      return *this;
    }

    Degree
    operator+(const Degree & d) const {
      return Degree(m_degree + d.m_degree);
    }

    Degree
    operator+(const Radian & r) const;

    Degree&
    operator+=(const Degree & d) {
      m_degree += d.m_degree;
      return *this;
    }

    Degree&
    operator+=(const Radian & r);

    Degree
    operator-() const {
      return Degree(-m_degree);
    }

    Degree
    operator-(const Degree & d) const {
      return Degree(m_degree - d.m_degree);
    }

    Degree
    operator-(const Radian & r) const;

    Degree&
    operator-=(const Degree & d) {
      m_degree -= d.m_degree;
      return *this;
    }

    Degree&
    operator-=(const Radian & r);

    Degree
    operator*(float f) const {
      return Degree(m_degree * f);
    }

    Degree
    operator*(const Degree & f) const {
      return Degree(m_degree * f.m_degree);
    }

    Degree&
    operator*=(float f) {
      m_degree *= f;
      return *this;
    }

    Degree
    operator/(float f) const {
      return Degree(m_degree / f);
    }

    Degree&
    operator/=(float f) {
      m_degree /= f;
      return *this;
    }

    bool
    operator<(const Degree & d) const {
      return m_degree < d.m_degree;
    }

    bool
    operator<=(const Degree & d) const {
      return m_degree <= d.m_degree;
    }

    bool
    operator==(const Degree & d) const {
      return m_degree == d.m_degree;
    }

    bool
    operator!=(const Degree & d) const {
      return m_degree != d.m_degree;
    }

    bool
    operator>=(const Degree & d) const {
      return m_degree >= d.m_degree;
    }

    bool 
    operator>(const Degree & d) const {
      return m_degree > d.m_degree;
    }

    bool
    operator<(const float& v) const {
      return m_degree < v;
    }

    bool
    operator<=(const float& v) const {
      return m_degree <= v;
    }

    bool
    operator==(const float& v) const {
      return m_degree == v;
    }

    bool
    operator!=(const float& v) const {
      return m_degree != v;
    }

    bool
    operator>=(const float& v) const {
      return m_degree >= v;
    }

    bool
    operator>(const float& v) const {
      return m_degree > v;
    }

    friend Degree
    operator*(float lhs, const Degree & rhs) {
      return Degree(lhs * rhs.m_degree);
    }

    friend Degree
    operator/(float lhs, const Degree & rhs) {
      return Degree(lhs / rhs.m_degree);
    }

    friend Degree
    operator+(Degree & lhs, float rhs) {
      return Degree(lhs.m_degree + rhs);
    }

    friend Degree
    operator+(float lhs, const Degree & rhs) {
      return Degree(lhs + rhs.m_degree);
    }

    friend Degree
    operator-(const Degree & lhs, float rhs) {
      return Degree(lhs.m_degree - rhs);
    }

    friend Degree
    operator-(const float lhs, const Degree & rhs) {
      return Degree(lhs - rhs.m_degree);
    }

    friend bool
    operator<(float lhs, const Degree & rhs) {
      return lhs < rhs.m_degree;
    }

    friend bool
    operator<=(float lhs, const Degree & rhs) {
      return lhs <= rhs.m_degree;
    }

    friend bool
    operator==(float lhs, const Degree & rhs) {
      return lhs == rhs.m_degree;
    }

    friend bool
    operator!=(float lhs, const Degree & rhs) {
      return lhs != rhs.m_degree;
    }

    friend bool
    operator>=(float lhs, const Degree & rhs) {
      return lhs >= rhs.m_degree;
    }

    friend bool
    operator>(float lhs, const Degree & rhs) {
      return lhs > rhs.m_degree;
    }

   private:
    float m_degree = 0.0f;
  };
}
