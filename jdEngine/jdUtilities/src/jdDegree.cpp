#include "jdDegree.h"
#include "jdRadian.h"

namespace jdEngineSDK {

  Degree::Degree(const Radian& r) : m_degree(r.valueDegrees()) {}

  Degree&
  Degree::operator=(const Radian& r) {
    m_degree = r.valueDegrees();
    return *this;
  }

  Degree
  Degree::operator+(const Radian& r) const {
    return Degree(m_degree + r.valueDegrees());
  }

  Degree&
  Degree::operator+=(const Radian& r) {
    m_degree += r.valueDegrees();
    return *this;
  }

  Degree
  Degree::operator-(const Radian& r) const {
    return Degree(m_degree - r.valueDegrees());
  }

  Degree&
  Degree::operator-=(const Radian& r) {
    m_degree -= r.valueDegrees();
    return *this;
  }

  float
    Degree::valueRadians() const {
    return m_degree * Math::DEG2RAD;
  }

  void
  Degree::unwindDegrees() {
    m_degree = Math::unwindDegrees(m_degree);
  }
}
