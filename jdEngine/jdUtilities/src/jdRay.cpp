#include "jdRay.h"

namespace jdEngineSDK {
  Ray::Ray(const JDVector3& origin, const JDVector3& direction, const float& distance) :
           m_origin(origin), m_direction(direction), m_distance(distance) {
    m_direction.normalize();
  }
}
