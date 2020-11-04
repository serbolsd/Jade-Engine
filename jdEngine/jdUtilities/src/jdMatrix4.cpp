#include "jdMatrix4.h"

namespace jdEngineSDK {

  JDMatrix4::JDMatrix4()
  {
    m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
    m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
    m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
  }
  
  JDMatrix4::JDMatrix4(const float src[16])
  {
    m[0] = src[0];   m[1] = src[1];  m[2] = src[2];   m[3] = src[3];
    m[4] = src[4];   m[5] = src[5];  m[6] = src[6];   m[7] = src[7];
    m[8] = src[8];   m[9] = src[9];  m[10] = src[10]; m[11] = src[11];
    m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
  }
  
  JDMatrix4::JDMatrix4(const float& m0, 
                       const float& m1, 
                       const float& m2, 
                       const float& m3, 
                       const float& m4, 
                       const float& m5, 
                       const float& m6, 
                       const float& m7,
                       const float& m8, 
                       const float& m9, 
                       const float& m10, 
                       const float& m11, 
                       const float& m12, 
                       const float& m13, 
                       const float& m14, 
                       const float& m15){
    m[0] = m0; m[1] = m1; m[2] = m2; m[3] = m3;
    m[4] = m4; m[5] = m5; m[6] = m6; m[7] = m7;
    m[8] = m8; m[9] = m9; m[10] = m10; m[11] = m11;
    m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
  }
  
  JDMatrix4::JDMatrix4(const JDVector4& m0123, 
                       const JDVector4& m4567, 
                       const JDVector4& m891011, 
                       const JDVector4& m12131415) {
    m[0] = m0123.x; m[1] = m0123.y; m[2] = m0123.z; m[3] = m0123.w;
    m[4] = m4567.x; m[5] = m4567.y; m[6] = m4567.z; m[7] = m4567.w;
    m[8] = m891011.x; m[9] = m891011.y; m[10] = m891011.z; m[11] = m891011.w;
    m[12] = m12131415.x; m[13] = m12131415.y; m[14] = m12131415.z; m[15] = m12131415.w;
  }
  
  JDMatrix4::JDMatrix4(const JDMatrix3& mat3) {
    m[0] = mat3.m[0]; m[1] = mat3.m[1]; m[2] = mat3.m[2]; m[3] = 0;
    m[4] = mat3.m[3]; m[5] = mat3.m[4]; m[6] = mat3.m[5]; m[7] = 0;
    m[8] = mat3.m[6]; m[9] = mat3.m[7]; m[10] = mat3.m[8]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1.0f;
  }
  
  JDMatrix4 
  JDMatrix4::get() const	{
    return *this;
  }
  
  JDMatrix4 
  JDMatrix4::getTranspose()	{
    JDMatrix4 tm = *this;
    tm.m[0] = m[0];   tm.m[1] = m[4];   tm.m[2] = m[8];   tm.m[3] = m[12];
    tm.m[4] = m[1];   tm.m[5] = m[5];   tm.m[6] = m[9];   tm.m[7] = m[13];
    tm.m[8] = m[2];   tm.m[9] = m[6];   tm.m[10] = m[10];  tm.m[11] = m[14];
    tm.m[12] = m[3];   tm.m[13] = m[7];   tm.m[14] = m[11];  tm.m[15] = m[15];
    return tm;
  }
  
  
  
  float 
  JDMatrix4::getDeterminant()	{
    float deter;
    JDMatrix3 deter0 =
    { m[5],m[6],m[7],
    m[9],m[10],m[11],
    m[13],m[14],m[15]
    };
    JDMatrix3 deter1 = {
    m[4],m[6],m[7],
    m[8],m[10],m[11],
    m[12],m[14],m[15]
    };
    JDMatrix3 deter2 = {
    m[4],m[5],m[7],
    m[8],m[9],m[11],
    m[12],m[13],m[15]
    };
    JDMatrix3 deter3 = {
    m[4],m[5],m[6],
    m[8],m[9],m[10],
    m[12],m[13],m[14]
    };
    deter = (m[0] * deter0.getDeterminant()) - (m[1] * deter1.getDeterminant()) + 
    		      (m[2] * deter2.getDeterminant()) - (m[3] * deter3.getDeterminant());
    return deter;
  }
  
  JDMatrix4& 
  JDMatrix4::identity()	{
    m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
    m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
    m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::transpose()	{
    std::swap(m[1], m[4]);
    std::swap(m[2], m[8]);
    std::swap(m[3], m[12]);
    std::swap(m[6], m[9]);
    std::swap(m[7], m[13]);
    std::swap(m[11], m[14]);
    
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::invert()	{
    float determinant, invDeterminant;
    JDMatrix3 mat0 = {
    m[5],m[6] ,m[7],
    m[9] ,m[10] ,m[11],
    m[13] ,m[14] ,m[15] };
    JDMatrix3 mat1 = {
    m[4],m[6] ,m[7],
    m[8] ,m[10] ,m[11],
    m[12] ,m[14] ,m[15] };
    JDMatrix3 mat2 = {
    m[4],m[5] ,m[7],
    m[8] ,m[9] ,m[11],
    m[12] ,m[13] ,m[15] };
    JDMatrix3 mat3 = {
    m[4],m[5] ,m[6],
    m[8] ,m[9] ,m[10],
    m[12] ,m[13] ,m[14] };
    JDMatrix3 mat4 = {
    m[1],m[2] ,m[3],
    m[9] ,m[10] ,m[11],
    m[13] ,m[14] ,m[15] };
    JDMatrix3 mat5 = {
    m[0],m[2] ,m[3],
    m[8] ,m[10] ,m[11],
    m[12] ,m[14] ,m[15] };
    JDMatrix3 mat6 = {
    m[0],m[1] ,m[3],
    m[8] ,m[9] ,m[11],
    m[12] ,m[13] ,m[15] };
    JDMatrix3 mat7 = {
    m[0],m[1] ,m[2],
    m[8] ,m[9] ,m[10],
    m[12] ,m[13] ,m[14] };
    JDMatrix3 mat8 = {
    m[1],m[2] ,m[3],
    m[5] ,m[6] ,m[7],
    m[13] ,m[14] ,m[15] };
    JDMatrix3 mat9 = {
    m[0],m[2] ,m[3],
    m[4] ,m[6] ,m[7],
    m[12] ,m[14] ,m[15] };
    JDMatrix3 mat10 = {
    m[0],m[1] ,m[3],
    m[4] ,m[5] ,m[7],
    m[12] ,m[13] ,m[15] };
    JDMatrix3 mat11 = {
    m[0],m[1] ,m[2],
    m[4] ,m[5] ,m[6],
    m[12] ,m[13] ,m[14] };
    JDMatrix3 mat12 = {
    m[1],m[2] ,m[3],
    m[5] ,m[6] ,m[7],
    m[9] ,m[10] ,m[11] };
    JDMatrix3 mat13 = {
    m[0],m[2] ,m[3],
    m[4] ,m[6] ,m[7],
    m[8] ,m[10] ,m[11] };
    JDMatrix3 mat14 = {
    m[0],m[1] ,m[3],
    m[4] ,m[5] ,m[7],
    m[8] ,m[9] ,m[11] };
    JDMatrix3 mat15 = {
    m[0],m[1] ,m[2],
    m[4] ,m[5] ,m[6],
    m[8] ,m[9] ,m[10] };
    determinant = getDeterminant();
    if (determinant >= -0.00001f && determinant <= 0.00001f) {
      return identity(); // cannot inverse, make it idenety matrix
    }
    invDeterminant = 1.0f / determinant;
    m[0] = (mat0.getDeterminant() * invDeterminant);
    m[1] = -(mat1.getDeterminant() * invDeterminant);
    m[2] = (mat2.getDeterminant() * invDeterminant);
    m[3] = -(mat3.getDeterminant() * invDeterminant);
    m[4] = -(mat4.getDeterminant() * invDeterminant);
    m[5] = (mat5.getDeterminant() * invDeterminant);
    m[6] = -(mat6.getDeterminant() * invDeterminant);
    m[7] = (mat7.getDeterminant() * invDeterminant);
    m[8] = (mat8.getDeterminant() * invDeterminant);
    m[9] = -(mat9.getDeterminant() * invDeterminant);
    m[10] = (mat10.getDeterminant() * invDeterminant);
    m[11] = -(mat11.getDeterminant() * invDeterminant);
    m[12] = -(mat12.getDeterminant() * invDeterminant);
    m[13] = (mat13.getDeterminant() * invDeterminant);
    m[14] = -(mat14.getDeterminant() * invDeterminant);
    m[15] = (mat15.getDeterminant() * invDeterminant);
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::translate(float x, float y, float z)	{
    JDMatrix4 tras =
    {
    	1, 0, 0, x,
    	0, 1, 0, y,
    	0, 0, 1, z,
    	0, 0, 0, 1
    };
    *this *= tras;
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::translate(const JDVector3& vec)	{
    translate(vec.x, vec.y, vec.z);
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotate(const Radian& angleRadian, const JDVector3& axis)	{
    float c = Math::cos(angleRadian.valueRadians());    // cosine
    float s = Math::sin(angleRadian.valueRadians());    // sine
    JDVector3 vec = axis;
    vec.normalize();
    JDMatrix4 result =	
      { c + ((vec.x * vec.x) * (1 - c)),((vec.x * vec.y) * (1 - c)) - (vec.z * s),
      ((vec.x * vec.z) * (1 - c)) + (vec.y * s), 0, 
      ((vec.x * vec.y) * (1 - c)) + (vec.z * s),
      c + ((vec.y * vec.y) * (1 - c)),	((vec.y * vec.z) * (1 - c)) - (vec.x * s),
      0, ((vec.x * vec.z) * (1 - c)) - (vec.y * s)	,
      ((vec.y * vec.z) * (1 - c)) + (vec.x * s),	c + ((vec.z * vec.z) * (1 - c)), 0,
      0,0,0,1};
    
    *this *= result;
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotate(const Radian& angleRadian, float x, float y, float z)	{
    rotate(angleRadian, JDVector3(x, y, z));
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotateX(const Radian& angleRadian)	{
    JDMatrix4 rotX =
    {1, 0, 0, 0,
     0, cosf(angleRadian.valueRadians()),-sinf(angleRadian.valueRadians()), 0,
     0, sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0,
     0, 0, 0, 1};
    
    *this = *this * rotX;
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotateXstayPos(const Radian& angleRadian)	{
    JDMatrix4 rotX =
    {1, 0, 0, 0,
     0, cosf(angleRadian.valueRadians()),-sinf(angleRadian.valueRadians()), 0,
     0, sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0,
     0, 0, 0, 1};
    
    *this = rotX * (*this);
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotateY(const Radian& angleRadian)	{
    JDMatrix4 rotY = 
    {cosf(angleRadian.valueRadians()), 0, sinf(angleRadian.valueRadians()), 0,
     0, 1, 0, 0,
     -sinf(angleRadian.valueRadians()), 0, cosf(angleRadian.valueRadians()), 0,
     0, 0, 0, 1};
    *this *= rotY;
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotateYstayPos(const Radian& angleRadian)	{
    JDMatrix4 rotY =	
    {Math::cos(angleRadian.valueRadians()), 0, Math::sin(angleRadian.valueRadians()), 0,
     0, 1, 0, 0,
     -Math::sin(angleRadian.valueRadians()), 0, Math::cos(angleRadian.valueRadians()), 0,
     0, 0, 0, 1};
    *this = rotY * (*this);
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotateZ(const Radian& angleRadian)	{
    JDMatrix4 rotZ = 
    {cosf(angleRadian.valueRadians()),-sinf(angleRadian.valueRadians()), 0, 0,
     sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1};
    
    *this *= rotZ;
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::rotateZstayPos(const Radian& angleRadian)	{
    JDMatrix4 rotZ = 
    {cosf(angleRadian.valueRadians()),-sinf(angleRadian.valueRadians()), 0, 0,
     sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1};
    *this = rotZ * (*this);
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::scale(float scal)	{
    scale(scal, scal, scal);
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::scale(float sx, float sy, float sz) {
    JDMatrix4 scal =
    {sx, 0, 0, 0,
     0, sy, 0, 0,
     0, 0, sz, 0,
     0, 0, 0, 1};
    
    *this *= scal;
    return *this;
  }

  JDMatrix4& 
  JDMatrix4::scale(const JDVector3& vec) {
    scale(vec.x, vec.y, vec.z);
    return *this;
  }
  
  JDMatrix4 
  JDMatrix4::operator+() const	{
    return *this;
  }
  
  JDMatrix4 
  JDMatrix4::operator-() const	{
    JDMatrix4 tmp = 
      { -m_00, -m_01, -m_02, -m_03,
       -m_10, -m_11, -m_02, -m_13,
       -m_20, -m_21, -m_02, -m_23,
       -m_30, -m_31, -m_02, -m_33 };
    return tmp;
  }
  
  JDMatrix4
  JDMatrix4::operator+(const JDMatrix4& mat) const {
    return JDMatrix4(
     m[0] + mat.m[0], m[1] + mat.m[1], m[0] + mat.m[2], m[3] + mat.m[3],
     m[4] + mat.m[4], m[5] + mat.m[5], m[6] + mat.m[6], m[7] + mat.m[7],
     m[8] + mat.m[8], m[9] + mat.m[9], m[10] + mat.m[10], m[11] + mat.m[11],
     m[12] + mat.m[12], m[13] + mat.m[13], m[14] + mat.m[14], m[15] + mat.m[15]);
  }
  
  JDMatrix4
  JDMatrix4::operator-(const JDMatrix4& mat) const	{
    return JDMatrix4(
      m[0] - mat.m[0], m[1] - mat.m[1], m[0] - mat.m[2], m[3] - mat.m[3],
      m[4] - mat.m[4], m[5] - mat.m[5], m[6] - mat.m[6], m[7] - mat.m[7],
      m[8] - mat.m[8], m[9] - mat.m[9], m[10] - mat.m[10], m[11] - mat.m[11],
      m[12] - mat.m[12], m[13] - mat.m[13], m[14] - mat.m[14], m[15] - mat.m[15]);
  }
  
  JDMatrix4& 
  JDMatrix4::operator+=(const JDMatrix4& mat)	{
    m[0] += mat.m[0]; m[1] += mat.m[1]; m[0] += mat.m[2]; m[3] += mat.m[3];
    m[4] += mat.m[4]; m[5] += mat.m[5]; m[6] += mat.m[6]; m[7] += mat.m[7];
    m[8] += mat.m[8]; m[9] += mat.m[9]; m[10] += mat.m[10]; m[11] += mat.m[11];
    m[12] += mat.m[12]; m[13] += mat.m[13]; m[14] += mat.m[14]; m[15] += mat.m[15];
    return *this;
  }
  
  JDMatrix4& 
  JDMatrix4::operator-=(const JDMatrix4& mat)	{
    m[0] -= mat.m[0]; m[1] -= mat.m[1]; m[0] -= mat.m[2]; m[3] -= mat.m[3];
    m[4] -= mat.m[4]; m[5] -= mat.m[5]; m[6] -= mat.m[6]; m[7] -= mat.m[7];
    m[8] -= mat.m[8]; m[9] -= mat.m[9]; m[10] -= mat.m[10]; m[11] -= mat.m[11];
    m[12] -= mat.m[12]; m[13] -= mat.m[13]; m[14] -= mat.m[14]; m[15] -= mat.m[15];
    return *this;
  }
  
  JDVector4 
  JDMatrix4::operator*(const JDVector4& vec) const	{
    return JDVector4(
      m[0] * vec.x + m[1] * vec.y + m[2] * vec.z + m[3] * vec.z,
      m[4] * vec.x + m[5] * vec.y + m[6] * vec.z + m[7] * vec.z,
      m[8] * vec.x + m[9] * vec.y + m[10] * vec.z + m[11] * vec.z,
      m[12] * vec.x + m[13] * vec.y + m[14] * vec.z + m[15] * vec.z);
  }
  
  JDVector3 
  JDMatrix4::operator*(const JDVector3& vec) const	{
    return JDVector3(
      m[0] * vec.x + m[1] * vec.y + m[2] * vec.z,
      m[4] * vec.x + m[5] * vec.y + m[6] * vec.z,
      m[8] * vec.x + m[9] * vec.y + m[10] * vec.z);
  }
  
  JDMatrix4 
  JDMatrix4::operator*(const JDMatrix4& mat) const	{
    return JDMatrix4(
      ((m[0] * mat.m[0]) + (m[1] * mat.m[4]) + (m[2] * mat.m[8]) + (m[3] * mat.m[12])), 
      ((m[0] * mat.m[1]) + (m[1] * mat.m[5]) + (m[2] * mat.m[9]) + (m[3] * mat.m[13])), 
      ((m[0] * mat.m[2]) + (m[1] * mat.m[6]) + (m[2] * mat.m[10]) + (m[3] * mat.m[14])), 
      ((m[0] * mat.m[3]) + (m[1] * mat.m[7]) + (m[2] * mat.m[11]) + (m[3] * mat.m[15])),
      ((m[4] * mat.m[0]) + (m[5] * mat.m[4]) + (m[6] * mat.m[8]) + (m[7] * mat.m[12])), 
      ((m[4] * mat.m[1]) + (m[5] * mat.m[5]) + (m[6] * mat.m[9]) + (m[7] * mat.m[13])), 
      ((m[4] * mat.m[2]) + (m[5] * mat.m[6]) + (m[6] * mat.m[10]) + (m[7] * mat.m[14])), 
      ((m[4] * mat.m[3]) + (m[5] * mat.m[7]) + (m[6] * mat.m[11]) + (m[7] * mat.m[15])),
      ((m[8] * mat.m[0]) + (m[9] * mat.m[4]) + (m[10] * mat.m[8]) + (m[11] * mat.m[12])), 
      ((m[8] * mat.m[1]) + (m[9] * mat.m[5]) + (m[10] * mat.m[9]) + (m[11] * mat.m[13])), 
      ((m[8] * mat.m[2]) + (m[9] * mat.m[6]) + (m[10] * mat.m[10]) + (m[11] * mat.m[14])), 
      ((m[8] * mat.m[3]) + (m[9] * mat.m[7]) + (m[10] * mat.m[11]) + (m[11] * mat.m[15])),
      ((m[12] * mat.m[0]) + (m[13] * mat.m[4]) + (m[14] * mat.m[8]) + (m[15] * mat.m[12])), 
      ((m[12] * mat.m[1]) + (m[13] * mat.m[5]) + (m[14] * mat.m[9]) + (m[15] * mat.m[13])), 
      ((m[12] * mat.m[2]) + (m[13] * mat.m[6]) + (m[14] * mat.m[10]) + (m[15] * mat.m[14])), 
      ((m[12] * mat.m[3]) + (m[13] * mat.m[7]) + (m[14] * mat.m[11]) + (m[15] * mat.m[15])));
  }
  
  JDMatrix4& 
  JDMatrix4::operator*=(const JDMatrix4& mat)	{
    *this = *this * mat;
    return *this;
  }
  
  bool 
  JDMatrix4::operator==(const JDMatrix4& mat) const	{
    return (m[0] == mat.m[0]) && (m[1] == mat.m[1]) && 
           (m[2] == mat.m[2]) && (m[3] == mat.m[3]) &&
           (m[4] == mat.m[4]) && (m[5] == mat.m[5]) && 
           (m[6] == mat.m[6]) && (m[7] == mat.m[7]) &&
           (m[8] == mat.m[8]) && (m[9] == mat.m[9]) && 
           (m[10] == mat.m[10]) && (m[11] == mat.m[11]) &&
           (m[12] == mat.m[12]) && (m[13] == mat.m[13]) && 
           (m[14] == mat.m[14]) && (m[15] == mat.m[15]);
  }
  
  bool 
  JDMatrix4::operator!=(const JDMatrix4& mat) const {
    return (m[0] != mat.m[0]) || (m[1] != mat.m[1]) || 
           (m[2] != mat.m[2]) || (m[3] != mat.m[3]) ||
           (m[4] != mat.m[4]) || (m[5] != mat.m[5]) || 
           (m[6] != mat.m[6]) || (m[7] != mat.m[7]) ||
           (m[8] != mat.m[8]) || (m[9] != mat.m[9]) || 
           (m[10] != mat.m[10]) || (m[11] != mat.m[11]) ||
           (m[12] != mat.m[12]) || (m[13] != mat.m[13]) || 
           (m[14] != mat.m[14]) || (m[15] != mat.m[15]);
  }
  
  float
  JDMatrix4::operator[](uint32 index) const	{
    JD_ASSERT(index < 16);
    return (&m_00)[index];
  }
  
  float&
  JDMatrix4::operator[](uint32 index) {
    JD_ASSERT(index < 16);
    return (&m_00)[index];
  }
  
  JDMatrix4 
  operator-(const JDMatrix4& mat) {
    return JDMatrix4(-mat[0], -mat[1], -mat[2], -mat[3],
                     -mat[4], -mat[5], -mat[6], -mat[7],
                     -mat[8], -mat[9], -mat[10], -mat[11],
                     -mat[12], -mat[13], -mat[14], -mat[15]);
  }
  
  std::ostream& 
  operator<<(std::ostream& os, const JDMatrix4& m)	{
    os << std::fixed << std::setprecision(5);
    os << "[" << std::setw(10) << m.m[0] << " " << std::setw(10) << m.m[1] << " " 
       << std::setw(10) << m.m[2] << " " << std::setw(10) << m.m[3] << "]\n"
       << "[" << std::setw(10) << m.m[4] << " " << std::setw(10) << m.m[5] << " " 
       << std::setw(10) << m.m[6] << " " << std::setw(10) << m.m[7] << "]\n"
       << "[" << std::setw(10) << m.m[8] << " " << std::setw(10) << m.m[9] << " " 
       << std::setw(10) << m.m[10] << " " << std::setw(10) << m.m[11] << "]\n"
       << "[" << std::setw(10) << m.m[12] << " " << std::setw(10) << m.m[13] << " " 
       << std::setw(10) << m.m[14] << " " << std::setw(10) << m.m[15] << "]\n";
    os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
    return os;
  }
  
  JDMatrix4 
  transposeMatrix(const JDMatrix4& mat) {
    return JDMatrix4(
      mat[0], mat[4], mat[8], mat[12],
      mat[1], mat[5], mat[9], mat[13],
      mat[2], mat[6], mat[10], mat[14],
      mat[3], mat[7], mat[11], mat[15]);
  }
  
  JDMatrix4 
  invertMatrix(const JDMatrix4& mat)	{
    float determinant, invDeterminant;
    JDMatrix3 mat0 = {
    mat[5],mat[6] ,mat[7],
    mat[9] ,mat[10] ,mat[11],
    mat[13] ,mat[14] ,mat[15] };
    JDMatrix3 mat1 = {
    mat[4],mat[6] ,mat[7],
    mat[8] ,mat[10] ,mat[11],
    mat[12] ,mat[14] ,mat[15] };
    JDMatrix3 mat2 = {
    mat[4],mat[5] ,mat[7],
    mat[8] ,mat[9] ,mat[11],
    mat[12] ,mat[13] ,mat[15] };
    JDMatrix3 mat3 = {
    mat[4],mat[5] ,mat[6],
    mat[8] ,mat[9] ,mat[10],
    mat[12] ,mat[13] ,mat[14] };
    JDMatrix3 mat4 = {
    mat[1],mat[2] ,mat[3],
    mat[9] ,mat[10] ,mat[11],
    mat[13] ,mat[14] ,mat[15] };
    JDMatrix3 mat5 = {
    mat[0],mat[2] ,mat[3],
    mat[8] ,mat[10] ,mat[11],
    mat[12] ,mat[14] ,mat[15] };
    JDMatrix3 mat6 = {
    mat[0],mat[1] ,mat[3],
    mat[8] ,mat[9] ,mat[11],
    mat[12] ,mat[13] ,mat[15] };
    JDMatrix3 mat7 = {
    mat[0],mat[1] ,mat[2],
    mat[8] ,mat[9] ,mat[10],
    mat[12] ,mat[13] ,mat[14] };
    JDMatrix3 mat8 = {
    mat[1],mat[2] ,mat[3],
    mat[5] ,mat[6] ,mat[7],
    mat[13] ,mat[14] ,mat[15] };
    JDMatrix3 mat9 = {
    mat[0],mat[2] ,mat[3],
    mat[4] ,mat[6] ,mat[7],
    mat[12] ,mat[14] ,mat[15] };
    JDMatrix3 mat10 = {
    mat[0],mat[1] ,mat[3],
    mat[4] ,mat[5] ,mat[7],
    mat[12] ,mat[13] ,mat[15] };
    JDMatrix3 mat11 = {
    mat[0],mat[1] ,mat[2],
    mat[4] ,mat[5] ,mat[6],
    mat[12] ,mat[13] ,mat[14] };
    JDMatrix3 mat12 = {
    mat[1],mat[2] ,mat[3],
    mat[5] ,mat[6] ,mat[7],
    mat[9] ,mat[10] ,mat[11] };
    JDMatrix3 mat13 = {
    mat[0],mat[2] ,mat[3],
    mat[4] ,mat[6] ,mat[7],
    mat[8] ,mat[10] ,mat[11] };
    JDMatrix3 mat14 = {
    mat[0],mat[1] ,mat[3],
    mat[4] ,mat[5] ,mat[7],
    mat[8] ,mat[9] ,mat[11] };
    JDMatrix3 mat15 = {
    mat[0],mat[1] ,mat[2],
    mat[4] ,mat[5] ,mat[6],
    mat[8] ,mat[9] ,mat[10] };
    JDMatrix4 result = mat;
    determinant = result.getDeterminant();
    if (determinant >= -0.00001f && determinant <= 0.00001f)
    {
      return result.identity(); // cannot inverse, make it idenety matrix
    }
    invDeterminant = 1.0f / determinant;
    result.m[0] = (mat0.getDeterminant() * invDeterminant);
    result.m[1] = -(mat1.getDeterminant() * invDeterminant);
    result.m[2] = (mat2.getDeterminant() * invDeterminant);
    result.m[3] = -(mat3.getDeterminant() * invDeterminant);
    result.m[4] = -(mat4.getDeterminant() * invDeterminant);
    result.m[5] = (mat5.getDeterminant() * invDeterminant);
    result.m[6] = -(mat6.getDeterminant() * invDeterminant);
    result.m[7] = (mat7.getDeterminant() * invDeterminant);
    result.m[8] = (mat8.getDeterminant() * invDeterminant);
    result.m[9] = -(mat9.getDeterminant() * invDeterminant);
    result.m[10] = (mat10.getDeterminant() * invDeterminant);
    result.m[11] = -(mat11.getDeterminant() * invDeterminant);
    result.m[12] = -(mat12.getDeterminant() * invDeterminant);
    result.m[13] = (mat13.getDeterminant() * invDeterminant);
    result.m[14] = -(mat14.getDeterminant() * invDeterminant);
    result.m[15] = (mat15.getDeterminant() * invDeterminant);
    return result;
  }
  
  JDMatrix4 
  identityMatrix()	{
    JDMatrix4 ident =	
      {1.0f, 0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f};
    return ident;
  }
  
  JDMatrix4 
  matrix4Translate(const float& x, const float& y, const float& z) {
   JDMatrix4 tras = matrix4Translate(JDVector3(x, y, z));
   return tras;
  }
  
  JDMatrix4
  matrix4Translate(const JDVector3& vec)	{
    JDMatrix4 tras = 
      {1.0f, 0.0f, 0.0f, vec.x,
       0.0f, 1.0f, 0.0f, vec.y,
       0.0f, 0.0f, 1.0f, vec.z,
       0.0f, 0.0f, 0.0f, 1.0f};
    return tras;
  }
  
  JDMatrix4 
  matrix4Rotate(const Radian& angleRadian, const JDVector3& axis)	{
    float c = cosf(angleRadian.valueRadians());    // cosine
    float s = sinf(angleRadian.valueRadians());    // sine
    JDVector3 vec = axis;
    vec.normalize();
    JDMatrix4 result = 
      {c + ((vec.x * vec.x) * (1.0f - c)), ((vec.x * vec.y) * (1.0f - c)) - (vec.z * s),
       ((vec.x * vec.z) * (1.0f - c)) + (vec.y * s), 0.0f,
       ((vec.x * vec.y) * (1.0f - c)) + (vec.z * s), c + ((vec.y * vec.y) * (1.0f - c)),
       ((vec.y * vec.z) * (1.0f - c)) - (vec.x * s), 0.0f,
       ((vec.x * vec.z) * (1.0f - c)) - (vec.y * s), ((vec.y *vec.z) * (1.0f-c)) + (vec.x * s),
       c + ((vec.z * vec.z) * (1.0f - c)), 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f};
    return result;
  }
  
  JDMatrix4 
  matrix4Rotate(const Radian& angleRadian, const float& x, const float& y, const float& z)	{
    JDMatrix4 result = matrix4Rotate(angleRadian, JDVector3(x, y, z));
    return result;
  }
  
  JDMatrix4 
  matrix4RotateX(const Radian& angleRadian)	{
    JDMatrix4 rotX =
      {1.0f, 0.0f, 0.0f, 0.0f,
       0.0f, cosf(angleRadian.valueRadians()),sinf(angleRadian.valueRadians()), 0.0f,
       0.0f, -sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f };
    
    return rotX;
  }
  
  JDMatrix4 
  matrix4RotateY(const Radian& angleRadian)	{
    JDMatrix4 rotY = 
      {cosf(angleRadian.valueRadians()), 0.0f, -sinf(angleRadian.valueRadians()), 0.0f,
       0.0f, 1.0, 0.0f, 0.0f,
       sinf(angleRadian.valueRadians()), 0.0f, cosf(angleRadian.valueRadians()), 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f };
    
    return rotY;
  }
  
  JDMatrix4 
  matrix4RotateZ(const Radian& angleRadian)	{
    JDMatrix4 rotZ = 
      {cosf(angleRadian.valueRadians()), sinf(angleRadian.valueRadians()), 0.0f, 0.0f,
      	-sinf(angleRadian.valueRadians()), cosf(angleRadian.valueRadians()), 0.0f, 0.0f,
      	0.0f, 0.0f, 1.0f, 0.0f,
      	0.0f, 0.0f, 0.0f, 1.0f };
    return rotZ;
  }
  
  JDMatrix4
  matrix4Scale(const float& scale)	{
    JDMatrix4 sca = matrix4Scale(scale, scale, scale);
    return sca;
  }
  
  JDMatrix4 
  matrix4Scale(const float& sx, const float& sy, const float& sz)	{
    JDMatrix4 sca = 
      {sx, 0.0f, 0.0f, 0.0f,
       0.0f, sy, 0.0f, 0.0f,
       0.0f, 0.0f, sz, 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f};

    return sca;
  }
  
  JD_UTILITY_EXPORT JDMatrix4 
  createProjectionPerspectiveMatrix(const float& fovAngle,
                                    const float& width, 
                                    const float& height,
                                    const float& _near, 
                                    const float& _far)	{
    return createProjectionPerspectiveMatrix(fovAngle,(width / height), _near, _far);
  }
  
  bool scalarNearEqual(const float& S1, const float& S2, const float& Epsilon) {
    float Delta = S1 - S2;
    uint32  AbsDelta = *(uint32*)&Delta & 0x7FFFFFFF;
    return (*(float*)&AbsDelta <= Epsilon);
  }

  JD_UTILITY_EXPORT JDMatrix4 
  createProjectionPerspectiveMatrix(const float& fovAngle,
                                    const float& aspectRatio, 
                                    const float& _near, 
                                    const float& _far)	{
    //JDMatrix4 Perpective;
    //// set the basic projection matrix
    //float scale = 1 / tan(aspectRatio * 0.5f * Math::PI / 180);
    //Perpective.M[0][0] = scale; // scale the x coordinates of the projected point 
    //Perpective.M[1][1] = scale; // scale the y coordinates of the projected point 
    //Perpective.M[2][2] = -_far / (_far - _near); // used to remap z to [0,1] 
    //Perpective.M[3][2] = -_far * _near / (_far - _near); // used to remap z [0,1] 
    //Perpective.M[2][3] = -1; // set w = -z 
    //Perpective.M[3][3] = 0;
    
    float SinFov;
    float CosFov;
    float Height;
    float Width;
    JDMatrix4 M;

    JD_ASSERT(!scalarNearEqual(fovAngle, 0.0f, 0.00001f * 2.0f));
    JD_ASSERT(!scalarNearEqual(aspectRatio, 0.0f, 0.00001f));
    JD_ASSERT(!scalarNearEqual(_far, _near, 0.00001f));

    SinFov = Math::sin(0.5f * fovAngle);
    CosFov = Math::cos(0.5f * fovAngle);

    Height = CosFov / SinFov;
    Width = Height / aspectRatio;

    M[0] = Width; M[1] = 0.0f; M[2] = 0.0f; M[3] = 0.0f;
    M[4] = 0.0f; M[5] = Height; M[6] = 0.0f; M[7] = 0.0f;
    M[8] = 0.0f; M[9] = 0.0f; M[10] = _far / (_far - _near); M[11] = 1.0f;
    M[12] = 0.0f; M[13] = 0.0f; M[14] = -M[10] * _near; M[15] = 0.0f;

    return M;
  }
  
  JD_UTILITY_EXPORT JDMatrix4 
  createProjectionOrthographicMatrix(const float& botton, 
                                     const float& top, 
                                     const float& left, 
                                     const float& right, 
                                     const float& _near,
                                     const float& _far)	{
    JDMatrix4 Orthographic;
    Orthographic.M[0][0] = 2 / (right - left);
    Orthographic.M[0][1] = 0;
    Orthographic.M[0][2] = 0;
    Orthographic.M[0][3] = 0;
    
    Orthographic.M[1][0] = 0;
    Orthographic.M[1][1] = 2 / (top - botton);
    Orthographic.M[1][2] = 0;
    Orthographic.M[1][3] = 0;
    
    Orthographic.M[2][0] = 0;
    Orthographic.M[2][1] = 0;
    Orthographic.M[2][2] = -2 / (_far - _near);
    Orthographic.M[2][3] = 0;
    
    Orthographic.M[3][0] = -(right + left) / (right - left);
    Orthographic.M[3][1] = -(top + botton) / (top - botton);
    Orthographic.M[3][2] = -(_far + _near) / (_far - _near);
    Orthographic.M[3][3] = 1;
    
    return Orthographic;
  }
  
  JD_UTILITY_EXPORT JDMatrix4 
  createViewMatrix(const JDVector4& _eye, const JDVector4& _at, const JDVector4& _up)	{
    //to left hand
    JDMatrix4 view;
    JDVector4 cameraFront = _at - _eye;
    cameraFront.normalize();
    
    JDVector4 cameraRight = _up.cross(cameraFront);
    cameraRight.normalize();
    
    JDVector4 realUp = cameraFront.cross(cameraRight);
    //realUp.normalize();
    JDVector4 up = realUp;
    view = {
    	cameraRight.x,cameraRight.y,cameraRight.z,0.f,
    	up.x,up.y,up.z,0.f,
    	cameraFront.x,cameraFront.y,cameraFront.z,0.f,
    	0.f,0.f,0.f,1.f
    };
    
    JDMatrix4 POS = {
    	1.f,0.f,0.f,-_eye.x,
    	0.f,1.f,0.f,-_eye.y,
    	0.f,0.f,1.f,-_eye.z,
    	0.f,0.f,0.f,1.f
    };
    view *= POS;
    view.transpose();
    return view;
  }
}