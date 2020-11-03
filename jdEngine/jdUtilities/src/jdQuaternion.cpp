#include "jdQuaternion.h"
#include "jdDegree.h"

namespace jdEngineSDK {

  Quaternion::Quaternion(const float& _x, const float& _y, const float& _z) : 
    x(_x), y(_y), z(_z), w(0.0f) {}

  Quaternion::Quaternion(const Radian& _x, const Radian& _y, const Radian& _z) {
    float fSinPitch = Math::taylorSin(_x.valueRadians() * 0.5f);
    float fCosPitch = Math::taylorCos(_x.valueRadians() * 0.5f);
    float fSinYaw = Math::taylorSin(_y.valueRadians() * 0.5f);
    float fCosYaw = Math::taylorCos(_y.valueRadians() * 0.5f);
    float fSinRoll = Math::taylorSin(_z.valueRadians() * 0.5f);
    float fCosRoll = Math::taylorCos(_z.valueRadians() * 0.5f);
    float fCosPitchCosYaw(fCosPitch * fCosYaw);
    float fSinPitchSinYaw(fSinPitch * fSinYaw);
    z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
  }

  Quaternion::Quaternion(const Degree& _x, const Degree& _y, const Degree& _z) {
    float fSinPitch = Math::taylorSin(_x.valueRadians() * 0.5f);
    float fCosPitch = Math::taylorCos(_x.valueRadians() * 0.5f);
    float fSinYaw = Math::taylorSin(_y.valueRadians() * 0.5f);
    float fCosYaw = Math::taylorCos(_y.valueRadians() * 0.5f);
    float fSinRoll = Math::taylorSin(_z.valueRadians() * 0.5f);
    float fCosRoll = Math::taylorCos(_z.valueRadians() * 0.5f);
    float fCosPitchCosYaw(fCosPitch * fCosYaw);
    float fSinPitchSinYaw(fSinPitch * fSinYaw);
    z = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    x = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    y = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
  }

  Quaternion::Quaternion(JDVector3 xyz, const float& w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w){
  }

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
    Quaternion tmpQuaternion = *this * quaternion;
    *this = tmpQuaternion;
    return *this;
  }

  Quaternion 
  Quaternion::operator*(const Quaternion& quaternion) const {
    Quaternion tmpQuaternion;
    tmpQuaternion.x = w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y;
    tmpQuaternion.y = w * quaternion.y + y * quaternion.w + z * quaternion.x - x * quaternion.z;
    tmpQuaternion.z = w * quaternion.z + z * quaternion.w + x * quaternion.y - y * quaternion.x;
    tmpQuaternion.w = w * quaternion.w - x * quaternion.x - y * quaternion.y - z * quaternion.z;
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
    return Math::sqrt(squareMagnitude());
  }

  float Quaternion::squareMagnitude() const
  {
    return x * x + y * y + z * z + w * w;
  }

  Quaternion& 
  Quaternion::normalize() {
    *this = getNormalize();
    return *this;
  }

  Quaternion 
  Quaternion::getNormalize() const {
    float inverMag = squareMagnitude();
    JD_ASSERT(0.0f <= inverMag);
   
    inverMag = Math::invSqrt(inverMag);
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
    return Quaternion(invSMagnitude, scalar);
  }

  Quaternion& 
  Quaternion::rotate(const float& _x, const float& _y, const float& _z, bool degree) {
    if (!degree)
    {
      return rotate(_x * Math::RAD2DEG, _y * Math::RAD2DEG, _z * Math::RAD2DEG);
    }
    else
    {
      return rotate(_x, _y, _z);
    }
  }

  Quaternion&
  Quaternion::rotate(Degree _x, Degree _y, Degree _z) {
    Quaternion q2(_x, _y, _z), q = *this, qinv = q;
    qinv.conjugate();

    *this = q * q2 * qinv;
    return *this;
  }

  JDVector3 
  Quaternion::getEuler() {
    JDVector3 euler;

    euler.z = Math::atan(2 * (x * y + w * z), w * w + x * x - y * y - z * z);
    euler.z *= Math::RAD2DEG;
    euler.y = Math::asin(-2 * (x * z - w * y));
    euler.y *= Math::RAD2DEG;
    euler.x = Math::atan((2) * (y * z + w * x), w * w - x * x - y * y + z * z);
    euler.x *= Math::RAD2DEG;

    return euler;
  }

  JDMatrix4 Quaternion::getMatrix() const
  {
    JDMatrix4 mat;
    mat.m[0] = 1.0f - 2.0f * (y * y + z * z); mat.m[1] = 2.0f * (x * y - z * w);
    mat.m[2] = 2.0f * (x * z + y * w); mat.m[3] = 0;

    mat.m[4] = 2.0f* (x * y + z * w); mat.m[5] = 1.0f - 2.0f* (x * x + z * z); 
    mat.m[6] = 2.0f* (y * z - x * w); mat.m[7] = 0;
    
    mat.m[8] = 2.0f * (x * z - y * w); mat.m[9] = 2.0f * (y * z + x * w); 
    mat.m[10] = 1.0f - 2.0f * (x * x + y * y); mat.m[11] = 0;
    
    mat.m[12] = 0; mat.m[13] = 0; mat.m[14] = 0; mat.m[15] = 1;
    return mat;
  }

}
