/*****************************************************************************/
/**
 * @file    jdVector3.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/JUN/2020
 * @brief   Structure for float points in 3-d space.
 *
 * Structure for float points in 3-d space.
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPlatformDefines.h"
#include "jdVector2.h"

namespace jdEngineSDK {

  class JDVector3
  {
/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /**
     * @brief defaul constructor
     */				
    JDVector3() = default;
    
    /**
     * @brief constructor with float params with default 0.
     * @param _x for x cordinate
     * @param _y for y cordinate
     * @param _z for z cordinate
     */
    JDVector3(const float& _x, const float& _y = 0.0f, const float& _z =0.0f) 
    		: x(_x), y(_y), z(_z) {};
    
    /**
     * @brief constructor with a array float 4 params.
     * @param _xyz for x, y ,z cordinate
     */
    JDVector3(const float _xyz[3]) : x(_xyz[0]), y(_xyz[1]), z(_xyz[2]) {};
    
    /**
     * @brief constructor with a JDVector2 and float params.
     * @param _xy for x, y cordinate
     * @param _z for z cordinate
     */
    JDVector3(const JDVector2& _xy, const float& _z = 0.0f) : x(_xy.x), y(_xy.y), z(_z) {};
    
    /**
     * @brief constructor with a JDVector3 and float params.
     * @param vec for x, y, z cordinate
     */
    JDVector3(const JDVector3& vec) : x(vec.x), y(vec.y), z(vec.z) {};
    
/*****************************************************************************/
/**
 * Operators
 */
/*****************************************************************************/
   public:
    /**
     * @brief Negative operator
     * @return A new point negative of this vector
     */
    FORCEINLINE JDVector3
    operator-() const;
    
    /**
     * @brief Positive operator
     * @return A new point positive of this vector
     */
    FORCEINLINE JDVector3
    operator+() const;
    
    /**
     * @brief add two vectors
     * @param vec is the othe vector to add
     * @return the addition of this vector with other
     */
    FORCEINLINE JDVector3     
    operator+(const JDVector3& Vec) const;
    
    /**
     * @brief substraction of two vectors
     * @param vec is the othe vector to subtrac
     * @return the substraction of this vector with other
     */
    FORCEINLINE JDVector3
    operator-(const JDVector3& Vec) const;
    
    /**
     * @brief modificque this vector adding other vector
     * @param vec is the othe vector to add
     * @return the addition of this vector with other in this vector
     */
    FORCEINLINE JDVector3&
    operator+=(const JDVector3& Vec); 
    
    /**
     * @brief modificque this vector subtacting other vector
     * @param vec is the othe vector to subtrac
     * @return the substraction of this vector with other in this vector
     */
    FORCEINLINE JDVector3&
    operator-=(const JDVector3& Vec);
    
    /**
     * @brief Get the result of scaling on this vector.
     * @param scale What to multiply the vector by.
     * @return A new scaled vector.
     */
    FORCEINLINE JDVector3
    operator*(const float& scale) const;
    
    /**
     * @brief Get the result of multiply this vector with other vector.
     * @param vec What to multiply the vector by.
     * @return the result of the multiply this vector with other.
     */
    FORCEINLINE JDVector3
    operator*(const JDVector3& Vec) const; 
    
    /**
     * @brief Scale this vector
     * @param Scale What to multiply this vector by.
     * @return the result of the multiply in this vector.
     */
    FORCEINLINE JDVector3&
    operator*=(const float& scale);
    
    /**
     * @brief multiply this vector with other vector.
     * @param vec What to multiply this vector by.
     * @return the result of the multiply this vector with other in this vector.
     */
    FORCEINLINE JDVector3&
    operator*=(const JDVector3& Vec);      
    
    /**
     * @brief divition of vector with a scale.
     * @param scale What to divide the vector by.
     * @return the result of the dividetion.
     */
    FORCEINLINE JDVector3
    operator/(const float& scale) const;		
    
    /**
     * @brief divition of vector with an other vector.
     * @param vec is a vector What to divide the vector by.
     * @return a vector with the result of the dividetion.
     */
    FORCEINLINE JDVector3
    operator/(const JDVector3& vec) const;
    
    /**
     * @brief divition of this vector with a scale.
     * @param scale What to divide this vector by.
     * @return the result of the dividetion in this vector.
     */
    FORCEINLINE JDVector3&
    operator/=(const float& scale);
    
    /**
     * @brief divition of this vector with an other vector.
     * @param vec is vector What to divide this vector by.
     * @return the result of the dividetion in this vector.
     */
    FORCEINLINE JDVector3&
    operator/=(const JDVector3& vec);
    
    /**
     * @brief compare a vector with an other vector.
     * @param vec is vector What to compare vector by.
     * @return true if all components are equal
     */
    FORCEINLINE bool
    operator==(const JDVector3& Vec) const;
    
    /**
     * @brief compare a vector with an other vector.
     * @param vec is vector What to compare vector by.
     * @return true if one component is diferent
     */
    FORCEINLINE bool
    operator!=(const JDVector3& Vec) const;
    
    /**
     * @brief compare a vector with an other vector.
     * @param vec is vector What to compare vector by.
     * @return true if one component is less
     */
    FORCEINLINE bool
    operator<(const JDVector3& Vec) const;	
    
    /**
     * @brief compare a vector with an other vector.
     * @param vec is vector What to compare vector by.
     * @return true if one component is major
     */
    FORCEINLINE bool
    operator>(const JDVector3& Vec) const;
    
    /**
     * @brief get the component in a index
     * @param index is index of component in the vector.
     * @return a const of the component in the index
     */
    FORCEINLINE float
    operator[](uint32 index) const;	
    
    /**
     * @brief get the component in a index
     * @param index is index of component in the vector.
     * @return a reference of the component in the index
     */
    FORCEINLINE float&
    operator[](uint32 index);
    
    /**
     * @brief queal this vector to other vector.
     * @param vec is vector What to equal vector by.
     * @return this vector equaled
     */
    FORCEINLINE JDVector3&
    operator=(const JDVector3& Vec);	 	 

/*****************************************************************************/
/**
 * Friends
 */
/*****************************************************************************/

    /**
     * @brief multiplicate the components of a vector for a constant
     * @param scale multply the components
     * @return the result of multiplication.
     */
    FORCEINLINE friend JDVector3
    operator*(const float& a, const JDVector3 vec);
    		 						
    /**
     * @brief out stream the vector
     * @return the values of the vector.
     */
    FORCEINLINE friend std::ostream&
    operator<<(std::ostream& os, const JDVector3& vec);

/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/
   public:

    /**
     * @brief function to get the magnitude of vector
     * @return a flot with the magnitud of vector
     */
    FORCEINLINE float
    magnitude() const;
    
    /**
     * @brief function to get the square of the magnitude of vector
     * @return a flot with the square of the magnitud of vector
     */
    FORCEINLINE float
    squareMagnitude() const;
    
    /**
     * @brief function to get the square of the magnitude of vector
     * @param vec is the vectir to
     * @return a flot with the square of the magnitud of vector
     */
    FORCEINLINE float
    distance(const JDVector3& vec) const;
    
    /**
     * @brief function to normalize this vector
     * @return this vector normalized
     */
    FORCEINLINE JDVector3&
    normalize();	
    
    /**
     * @brief function to get the normalize of this vector
     * @return a copy of this vector normalized
     */
    FORCEINLINE JDVector3
    getnormalize();			
    
    /**
     * @brief dot product
     * @param vec if the vector with do dot product
     * @return the result of dot product
     */
    FORCEINLINE float
    dot(const JDVector3& vec) const;	
    
    /**
     * @brief dot product
     * @param vec if the vector with do cross product
     * @return the result of cross product
     */
    FORCEINLINE JDVector3
    cross(const JDVector3& vec) const;	

/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
  public:
      /**
       * @brief Vector's x component.
       */
      float x;
      /**
       * @brief Vector's y component.
       */
      float y;
      /**
       * @brief Vector's z component.
       */
      float z;
  };
  
  FORCEINLINE JDVector3 
  JDVector3::operator-() const {
  	 return JDVector3(-x, -y, -z);
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator+() const {
  		return *this;
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator+(const JDVector3& Vec) const {
  	 return JDVector3(x + Vec.x, y + Vec.y, z + Vec.z);
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator-(const JDVector3& Vec) const {
  	 return JDVector3(x - Vec.x, y - Vec.y, z - Vec.z);
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator+=(const JDVector3& Vec) {
  	 x += Vec.x; y += Vec.y; z += Vec.z;
  	 return *this;
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator-=(const JDVector3& Vec) {
  	 x -= Vec.x; y -= Vec.y; z -= Vec.z;
  	 return *this;
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator*(const float& scale) const {
   	return JDVector3(x * scale, y * scale, z * scale);
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator*(const JDVector3& Vec) const {
  	 return JDVector3(x * Vec.x, y * Vec.y, z * Vec.z);
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator*=(const float& scale) {
  	 x *= scale; y *= scale; z *= scale;
  	 return *this;
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator*=(const JDVector3& Vec) {
  	 x *= Vec.x; y *= Vec.y; z *= Vec.z;
  	 return *this;
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator/(const float& scale) const {
  	 return JDVector3(x / scale, y / scale, z / scale);
  }
  
  FORCEINLINE JDVector3 
  JDVector3::operator/(const JDVector3& vec) const	{
  		return JDVector3(x / vec.x, y / vec.y, z / vec.z);
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator/=(const float& scale) {
  	 x /= scale; y /= scale; z /= scale;
  	 return *this;
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator/=(const JDVector3& vec)	{
  		x /= vec.x; y /= vec.y; z /= vec.z;
  		return *this;
  }
  
  FORCEINLINE bool 
  JDVector3::operator==(const JDVector3& Vec) const {
   	return (x == Vec.x) && (y == Vec.y) && (z == Vec.z);
  }
  
  FORCEINLINE bool 
  JDVector3::operator!=(const JDVector3& Vec) const {
   	return (x != Vec.x) || (y != Vec.y) || (z != Vec.z);
  }
  
  FORCEINLINE bool 
   JDVector3::operator<(const JDVector3& Vec) const {
   if (x < Vec.x || y < Vec.y || z < Vec.z) { 
  	 return true; 
   }
  	 return false;
  }
  
  FORCEINLINE bool 
  JDVector3::operator>(const JDVector3& Vec) const	{
  	 if (x > Vec.x || y > Vec.y || z > Vec.z) {
  	   return true;
  	 }
  	 return false;
  }
  
  FORCEINLINE float 
  JDVector3::operator[](uint32 index) const {
   JD_ASSERT(index < 3);
     return (&x)[index];
  }
  
  FORCEINLINE float& 
  JDVector3::operator[](uint32 index) {
    JD_ASSERT(index < 3);
    return (&x)[index];
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::operator=(const JDVector3& Vec) {
  	 x = Vec.x; y = Vec.y; z = Vec.z;
  	 return *this;
  }
  
  FORCEINLINE JDVector3 
  operator*(const float& a, const JDVector3 vec) {
   	return JDVector3(a * vec.x, a * vec.y, a * vec.z);
  }
  
  FORCEINLINE std::ostream& 
  operator<<(std::ostream& os, const JDVector3& vec) {
  	 os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  	 return os;
  }
  
  FORCEINLINE float 
  JDVector3::magnitude() const {
   	return Math::sqrt(x * x + y * y + z * z);
  }
  
  FORCEINLINE float 
  JDVector3::squareMagnitude() const {
    float result = x * x + y * y + z * z;
    if (result < 0)
    {
      result = 0.000001f;
    }
   	return result;
  }
  
  FORCEINLINE float 
  JDVector3::distance(const JDVector3& vec) const {
   	return Math::sqrt((x-vec.x ) * (x - vec.x) + (y - vec.y) * 
  				           (y - vec.y) + (z-vec.z) * (z-vec.z));
  }
  
  FORCEINLINE JDVector3& 
  JDVector3::normalize() {
  	 *this = getnormalize();
  	 return *this;
  }
  
  FORCEINLINE JDVector3 
  JDVector3::getnormalize() {
   	  float sMag = squareMagnitude();
  	JD_ASSERT(0.0f <= sMag);
   	  JDVector3 result = *this;
  
   	  float invLength = Math::invSqrt(sMag);
   	  result.x *= invLength;
   	  result.y *= invLength;
   	  result.z *= invLength;
   	  return result;		
  }
  
  FORCEINLINE float 
  JDVector3::dot(const JDVector3& vec) const {
  	 return (x * vec.x + y * vec.y + z * vec.z);
  }
  		
  FORCEINLINE JDVector3 
  JDVector3::cross(const JDVector3& vec) const {
   	return JDVector3(y * vec.z - z * vec.y, -(x * vec.z - z * vec.x), x * vec.y - y * vec.x);
  }
}
