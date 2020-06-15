#include "jdSphere.h"

namespace jdEngineSDK {
  bool 
  Sphere::operator==(const Sphere& other) const {
    if (Math::abs(getVolume() - other.getVolume()) < Math::EPSILONF) {
      return true;
    }
    return false;
  }

  bool 
  Sphere::operator!=(const Sphere& other) const {
    if (Math::abs(getVolume() - other.getVolume()) > Math::EPSILONF) {
      return true;
    }
    return false;
  }
  
  float
  Sphere::getVolume() const {
    return (4.f / 3.f) * Math::PI * (m_radius * m_radius * m_radius);
  }

  void 
  Sphere::setCenter(const JDVector3& center) {
    m_center = center;
  }
  
  void Sphere::setRadius(const float& radius) {
    m_radius = radius;
  }
}
