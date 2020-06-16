#include "jdQuaternion.h"

namespace jdEngineSDK {

  Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& w) :
                         x(x), y(y), z(z), w(w) {}

  Quaternion::Quaternion(JDVector3 xyz, const float& w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w){}

  Quaternion::Quaternion(const Quaternion& quaternion) : x(quaternion.x), y(quaternion.y), 
                                                         z(quaternion.z), w(quaternion.w) {}

  Quaternion& 
  Quaternion::operator=(const Quaternion& quaternion) {
    x = quaternion.x;
    y = quaternion.y;
    z = quaternion.z;
    w = quaternion.w;
    return *this;
  }

  Quaternion&
  Quaternion::operator+=(const Quaternion& quaternion) {
    x += quaternion.x;
    y += quaternion.y;
    z += quaternion.z;
    w += quaternion.w;
    return *this;
  }

  Quaternion 
  Quaternion::operator+(const Quaternion& quaternion) const {
    Quaternion tmpQuaternion;
    tmpQuaternion.x = x + quaternion.x;
    tmpQuaternion.y = y + quaternion.y;
    tmpQuaternion.z = z + quaternion.z;
    tmpQuaternion.w = w + quaternion.w;
    return tmpQuaternion;
  }

  Quaternion& 
  Quaternion::operator-=(const Quaternion& quaternion) {
    x -= quaternion.x;
    y -= quaternion.y;
    z -= quaternion.z;
    w -= quaternion.w;
    return *this;
  }

  Quaternion 
  Quaternion::operator-(const Quaternion& quaternion) const {
    Quaternion tmpQuaternion;
    tmpQuaternion.x = x - quaternion.x;
    tmpQuaternion.y = y - quaternion.y;
    tmpQuaternion.z = z - quaternion.z;
    tmpQuaternion.w = w - quaternion.w;
    return tmpQuaternion;
  }

  Quaternion& 
  Quaternion::operator*=(const Quaternion& quaternion) {
    x *= quaternion.x;
    y *= quaternion.y;
    z *= quaternion.z;
    w *= quaternion.w;
    return *this;
  }

  Quaternion 
  Quaternion::operator*(const Quaternion& quaternion) const {
    Quaternion tmpQuaternion;
    tmpQuaternion.x = x * quaternion.x;
    tmpQuaternion.y = y * quaternion.y;
    tmpQuaternion.z = z * quaternion.z;
    tmpQuaternion.w = w * quaternion.w;
    return tmpQuaternion;
  }

  Quaternion& 
  Quaternion::operator*=(const float& scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
    return *this;
  }

  Quaternion Quaternion::operator*(const float& scale) const
  {
    Quaternion tmpQuaternion;
    tmpQuaternion.x = x * scale;
    tmpQuaternion.y = y * scale;
    tmpQuaternion.z = z * scale;
    tmpQuaternion.w = w * scale;
    return tmpQuaternion;
  }

  bool
  Quaternion::operator==(const Quaternion& quaternion) const {
    if (Math::abs(x - quaternion.x) < Math::EPSILONF &&
        Math::abs(y - quaternion.y) < Math::EPSILONF &&
        Math::abs(z - quaternion.z) < Math::EPSILONF &&
        Math::abs(w - quaternion.w) < Math::EPSILONF) {
      return true;
    }
    return false;
  }

  bool 
  Quaternion::operator!=(const Quaternion& quaternion) const {
    if (Math::abs(x - quaternion.x) > Math::EPSILONF ||
        Math::abs(y - quaternion.y) > Math::EPSILONF ||
        Math::abs(z - quaternion.z) > Math::EPSILONF ||
        Math::abs(w - quaternion.w) > Math::EPSILONF) {
      return true;
    }
    return false;
  }

  bool 
  Quaternion::operator<(const Quaternion& quaternion) const {
    if (magnitude() < quaternion.magnitude()) {
      return true;
    }
    return false;
  }

  bool 
  Quaternion::operator>(const Quaternion& quaternion) const {
    if (magnitude() > quaternion.magnitude()) {
      return true;
    }
    return false;
  }

  float 
  Quaternion::magnitude() const {
    return Math::Sqrt(x * x + y * y + z * z + w * w);
  }

  Quaternion& 
  Quaternion::normalize() {
    *this = getNormalize();
    return *this;
  }

  Quaternion 
  Quaternion::getNormalize() const {
    float inverMag = 1 / magnitude();
    Quaternion tmpQuaternion;
    tmpQuaternion.x = x * inverMag;
    tmpQuaternion.y = y * inverMag;
    tmpQuaternion.z = z * inverMag;
    tmpQuaternion.w = w * inverMag;
    return tmpQuaternion;
  }

  Quaternion& 
  Quaternion::conjugate() {
    *this = getConjugate();
    return *this;
  }

  Quaternion Quaternion::getConjugate() const
  {
    float scalar = w;
    JDVector3 tmpAxis = { x,y,z };
    tmpAxis *= -1;
    return Quaternion(tmpAxis, scalar);
  }

  Quaternion&
  Quaternion::inverse() {
    *this = getInverse();
    return *this;
  }

  Quaternion 
  Quaternion::getInverse() const {
    float invSMagnitude = magnitude();
    invSMagnitude *= invSMagnitude;
    invSMagnitude = 1 / invSMagnitude;

    Quaternion tmpConjugate = getConjugate();

    float scalar = tmpConjugate.w * invSMagnitude;
    JDVector3 tmpAxis = { tmpConjugate.x, tmpConjugate.y, tmpConjugate.z };
    tmpAxis *= invSMagnitude;
    return Quaternion(invSMagnitude, w);
  }

}
