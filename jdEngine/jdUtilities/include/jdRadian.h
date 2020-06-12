/*****************************************************************************/
/**
 * @file    jdRadian.h
 * @author Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date   8/JUN/2020
 * @brief   Wrapper class which indicates a given angle value is in Radians.
 *
 * Radian values are interchangeable with Degree values, and conversions will
 * be done automatically between them.
 *
 * @bug	    No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"

namespace jdEngineSDK
{
  class Degree;

  /**
   * @brief Wrapper class which indicates a given angle value is in Radians.
   * @note  Radian values are interchangeable with Degree values, and
   *        conversions will be done automatically between them.
   */
  class JD_UTILITY_EXPORT Radian
  {
  public:
    constexpr Radian() = default;

    constexpr Radian(const Radian&) = default;

    constexpr explicit Radian(float r) : m_radian(r) {}

    Radian(const Degree& d);

    constexpr Radian&
    operator=(const float& f) {
      m_radian = f;
      return *this;
    }

    constexpr Radian&
    operator=(const Radian&) = default;

    Radian&
    operator=(const Degree& d);

    FORCEINLINE float
    valueDegrees() const;

    FORCEINLINE float
    valueRadians() const {
      return m_radian;
    }

    /**
    * @brief Utility to ensure angle is between +/- PI range by unwinding.
    */
    void
      unwindRadians();

    const Radian&
      operator+() const {
      return *this;
    }

    Radian
      operator+(const Radian& r) const {
      return Radian(m_radian + r.m_radian);
    }

    Radian
      operator+(const Degree& d) const;

    Radian&
      operator+=(const Radian& r) {
      m_radian += r.m_radian;
      return *this;
    }

    Radian&
      operator+=(const Degree& d);

    Radian
      operator-() const {
      return Radian(-m_radian);
    }

    Radian
      operator-(const Radian& r) const {
      return Radian(m_radian - r.m_radian);
    }

    Radian
      operator-(const Degree& d) const;

    Radian&
      operator-=(const Radian& r) {
      m_radian -= r.m_radian;
      return *this;
    }

    Radian&
      operator-=(const Degree& d);

    Radian
      operator*(float f) const {
      return Radian(m_radian * f);
    }

    Radian
      operator*(const Radian& f) const {
      return Radian(m_radian * f.m_radian);
    }

    Radian&
      operator*=(float f) {
      m_radian *= f;
      return *this;
    }

    Radian
      operator/(float f) const {
      return Radian(m_radian / f);
    }

    Radian&
      operator/=(float f) {
      m_radian /= f;
      return *this;
    }

    bool
      operator<(const Radian& r) const {
      return m_radian < r.m_radian;
    }

    bool
      operator<=(const Radian& r) const {
      return m_radian <= r.m_radian;
    }

    bool
      operator==(const Radian& r) const {
      return m_radian == r.m_radian;
    }

    bool
      operator!=(const Radian& r) const {
      return m_radian != r.m_radian;
    }

    bool
      operator>=(const Radian& r) const {
      return m_radian >= r.m_radian;
    }

    bool
      operator>(const Radian& r) const {
      return m_radian > r.m_radian;
    }

    bool
      operator<(const float& v) const {
      return m_radian < v;
    }

    bool
      operator<=(const float& v) const {
      return m_radian <= v;
    }

    bool
      operator==(const float& v) const {
      return m_radian == v;
    }

    bool
      operator!=(const float& v) const {
      return m_radian != v;
    }

    bool
      operator>=(const float& v) const {
      return m_radian >= v;
    }

    bool operator>(const float& v) const {
      return m_radian > v;
    }

    friend Radian
      operator*(float lhs, const Radian& rhs) {
      return Radian(lhs * rhs.m_radian);
    }

    friend Radian
      operator/(float lhs, const Radian& rhs) {
      return Radian(lhs / rhs.m_radian);
    }

    friend Radian
      operator+(Radian& lhs, float rhs) {
      return Radian(lhs.m_radian + rhs);
    }

    friend Radian
      operator+(float lhs, const Radian& rhs) {
      return Radian(lhs + rhs.m_radian);
    }

    friend Radian
      operator-(const Radian& lhs, float rhs) {
      return Radian(lhs.m_radian - rhs);
    }

    friend Radian
      operator-(const float lhs, const Radian& rhs) {
      return Radian(lhs - rhs.m_radian);
    }

    friend bool
      operator<(float lhs, const Radian& rhs) {
      return lhs < rhs.m_radian;
    }

    friend bool
      operator<=(float lhs, const Radian& rhs) {
      return lhs <= rhs.m_radian;
    }

    friend bool
      operator==(float lhs, const Radian& rhs) {
      return lhs == rhs.m_radian;
    }

    friend bool
      operator!=(float lhs, const Radian& rhs) {
      return lhs != rhs.m_radian;
    }

    friend bool
      operator>=(float lhs, const Radian& rhs) {
      return lhs >= rhs.m_radian;
    }

    friend bool
      operator>(float lhs, const Radian& rhs) {
      return lhs > rhs.m_radian;
    }

  private:
    float m_radian = 0.0f;
  };
}
