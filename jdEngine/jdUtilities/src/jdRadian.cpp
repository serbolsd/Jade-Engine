#include "jdRadian.h"
#include "jdDegree.h"

namespace jdEngineSDK {
  Radian::Radian(const Degree & d) : m_radian(d.valueRadians()) {}

  Radian &
  Radian::operator=(const Degree & d) {
    m_radian = d.valueRadians();
    return *this;
  }

  Radian
  Radian::operator+(const Degree & d) const {
    return Radian(m_radian + d.valueRadians());
  }

  Radian &
  Radian::operator+=(const Degree & d) {
    m_radian += d.valueRadians();
    return *this;
  }

  Radian
  Radian::operator-(const Degree & d) const {
    return Radian(m_radian - d.valueRadians());
  }

  Radian &
  Radian::operator-=(const Degree & d) {
    m_radian -= d.valueRadians();
    return *this;
  }

  float
  Radian::valueDegrees() const {
    return m_radian * Math::RAD2DEG;
  }

  void
  Radian::unwindRadians() {
    m_radian = Math::unwindRadians(m_radian);
  }
}