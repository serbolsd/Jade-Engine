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
#pragma once

#include "jdPlatformDefines.h"
#include "jdVector2.h"

namespace jdEngineSDK {

	 class JDVector3
	 {
///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////
 		public:
				/**
					* @brief Default constructor (equal x and y to 0).
					*/
				FORCEINLINE JDVector3() : x(0.0f), y(0.0f), z(0.0f) {};

				/**
					* @brief constructor with float params with default 0.
					* @param _x for x cordinate
					* @param _y for y cordinate
					* @param _z for z cordinate
					*/
				FORCEINLINE JDVector3(const float& _x= 0.0f, const float& _y= 0.0f, 
						                    const float& _z=0.0f) : x(_x), y(_y), z(_z) {};

				/**
					* @brief constructor with a array float 4 params.
					* @param _xyz for x, y ,z cordinate
					*/
				FORCEINLINE JDVector3(const float _xyz[3]) : x(_xyz[0]), y(_xyz[1]), z(_xyz[2]) {};

				/**
					* @brief constructor with a JDVector2 and float params.
					* @param _xy for x, y cordinate
					* @param _z for z cordinate
					*/
				FORCEINLINE JDVector3(const JDVector2& _xy, const float& _z = 0.0f) : 
						                    x(_xy.x), y(_xy.y), z(_z) {};
				
				/**
					* @brief constructor with a JDVector3 and float params.
					* @param vec for x, y, z cordinate
					*/
				FORCEINLINE JDVector3(const JDVector3& vec) : x(vec.x), y(vec.y), z(vec.z) {};

	///////////////////////////////////////////////////////////////////////////////
	// Operators
	///////////////////////////////////////////////////////////////////////////////

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
	 	 JDVector3     
				operator+(const JDVector3& Vec) const;

				/**
					* @brief substraction of two vectors
					* @param vec is the othe vector to subtrac
					* @return the substraction of this vector with other
					*/
	 	 JDVector3     
				operator-(const JDVector3& Vec) const;
	 	 
				/**
					* @brief modificque this vector adding other vector
					* @param vec is the othe vector to add
					* @return the addition of this vector with other in this vector
					*/
				JDVector3&
				operator+=(const JDVector3& Vec); 
	 	 
				/**
					* @brief modificque this vector subtacting other vector
					* @param vec is the othe vector to subtrac
					* @return the substraction of this vector with other in this vector
					*/
				JDVector3&
				operator-=(const JDVector3& Vec);
	 	 
				/**
					* @brief Get the result of scaling on this vector.
					* @param scale What to multiply the vector by.
					* @return A new scaled vector.
					*/
				JDVector3
				operator*(const float& scale) const;
	 	 
				/**
					* @brief Get the result of multiply this vector with other vector.
					* @param vec What to multiply the vector by.
					* @return the result of the multiply this vector with other.
					*/
				JDVector3
				operator*(const JDVector3& Vec) const; 
	 	 
				/**
					* @brief Scale this vector
					* @param Scale What to multiply this vector by.
					* @return the result of the multiply in this vector.
					*/
				JDVector3&	
				operator*=(const float& scale);
	 	 
				/**
				 * @brief multiply this vector with other vector.
				 * @param vec What to multiply this vector by.
				 * @return the result of the multiply this vector with other in this vector.
				 */
				JDVector3&	
				operator*=(const JDVector3& Vec);      
	 	 
				/**
				* @brief divition of vector with a scale.
				* @param scale What to divide the vector by.
				* @return the result of the dividetion.
				*/
				JDVector3 
				operator/(const float& scale) const;		

				/**
					* @brief divition of vector with an other vector.
					* @param vec is a vector What to divide the vector by.
					* @return a vector with the result of the dividetion.
					*/
				JDVector3
				operator/(const JDVector3& vec) const;
	 	 
				/**
					* @brief divition of this vector with a scale.
					* @param scale What to divide this vector by.
					* @return the result of the dividetion in this vector.
					*/
				JDVector3&
				operator/=(const float& scale);

				/**
					* @brief divition of this vector with an other vector.
					* @param vec is vector What to divide this vector by.
					* @return the result of the dividetion in this vector.
					*/
				JDVector3&
				operator/=(const JDVector3& vec);
	 	
				/**
					* @brief compare a vector with an other vector.
					* @param vec is vector What to compare vector by.
					* @return true if all components are equal
					*/
				bool
				operator==(const JDVector3& Vec) const;
	 	 
				/**
				 * @brief compare a vector with an other vector.
				 * @param vec is vector What to compare vector by.
				 * @return true if one component is diferent
				 */
				bool
				operator!=(const JDVector3& Vec) const;
	 	 
				/**
					* @brief compare a vector with an other vector.
					* @param vec is vector What to compare vector by.
					* @return true if one component is less
					*/
				bool
				operator<(const JDVector3& Vec) const;	
	 	 
				/**
					* @brief compare a vector with an other vector.
					* @param vec is vector What to compare vector by.
					* @return true if one component is major
					*/
				bool
				operator>(const JDVector3& Vec) const;

				/**
					* @brief get the component in a index
					* @param index is index of component in the vector.
					* @return a const of the component in the index
					*/
				float
				operator[](int32 index) const;	
	 	 
				/**
					* @brief get the component in a index
					* @param index is index of component in the vector.
					* @return a reference of the component in the index
					*/
				float&			
				operator[](int32 index);					
	 	 
				/**
					* @brief queal this vector to other vector.
					* @param vec is vector What to equal vector by.
					* @return this vector equaled
					*/
				JDVector3&
				operator=(const JDVector3& Vec);	 	 

///////////////////////////////////////////////////////////////////////////////
// Friends
///////////////////////////////////////////////////////////////////////////////

				/**
					* @brief multiplicate the components of a vector for a constant
					* @param scale multply the components
					* @return the result of multiplication.
					*/
	 	 friend JDVector3 
				operator*(const float& a, const JDVector3 vec);
				
				/**
					* @brief out stream the vector
					* @return the values of the vector.
					*/
				friend std::ostream& 
				operator<<(std::ostream& os, const JDVector3& vec);

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////
			public:

				/**
					* @brief function to get the magnitude of vector
					* @return a flot with the magnitud of vector
					*/
				float       
				magnitude() const;
				
				/**
					* @brief function to get the square of the magnitude of vector
					* @return a flot with the square of the magnitud of vector
					*/
				float       
				squareMagnitude() const;
				
				/**
					* @brief function to get the square of the magnitude of vector
					* @param vec is the vectir to
					* @return a flot with the square of the magnitud of vector
					*/
				float   
				distance(const JDVector3& vec) const;

				/**
					* @brief function to normalize this vector
					* @return this vector normalized
					*/
				JDVector3& 
				normalize();	
				
				/**
					* @brief function to get the normalize of this vector
					* @return a copy of this vector normalized
					*/
				JDVector3 
				getnormalize();			

				/**
					* @brief dot product
					* @param vec if the vector with do dot product
					* @return the result of dot product
					*/
				float   
				dot(const JDVector3& vec) const;	

				/**
					* @brief dot product
					* @param vec if the vector with do cross product
					* @return the result of cross product
					*/
				JDVector3
				cross(const JDVector3& vec) const;	

///////////////////////////////////////////////////////////////////////////////
// Members
///////////////////////////////////////////////////////////////////////////////
			public:
				/**
					* @brief Vector's components.
					*/
				union
				{
						struct
						{
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
						/**
							* @brief Vector's components as array.
							*/
						float Vector[3];
				};
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
				if (x < Vec.x) return true;
				else return false;
	  	if (y < Vec.y) return true;
				else return false;
	 	 if (z < Vec.z) return true;
	 	 return false;
	 }
	 
		FORCEINLINE bool 
		JDVector3::operator>(const JDVector3& Vec) const	{
				if (x > Vec.x) return true;
				else return false;
				if (y > Vec.y) return true;
				else return false;
				if (z > Vec.z) return true;
				return false;
		}

		FORCEINLINE float 
		JDVector3::operator[](int32 index) const {
 	 	return (&x)[index];
	 }
	 
	 FORCEINLINE float& 
		JDVector3::operator[](int32 index) {
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
	  	return sqrtf(x * x + y * y + z * z);
	 }
	 
	 FORCEINLINE float 
		JDVector3::squareMagnitude() const {
	  	return (x * x + y * y + z * z);
	 }
	 
	 FORCEINLINE float 
		JDVector3::distance(const JDVector3& vec) const {
	  	return sqrtf((vec.x - x) * (vec.x - x) + (vec.y - y) * 
						           (vec.y - y) + (vec.z - z) * (vec.z - z));
	 }
	 
	 FORCEINLINE JDVector3& 
		JDVector3::normalize() {
	 	 float mod = squareMagnitude();
	 	 if (mod == 0)
	 	 {
	 	 	 x = 0;
	 	 	 y = 0;
	 	 	 z = 0;
	 	 }
	 	 else
	 	 {
	 	 	 float invLength = 1.0f / sqrtf(mod);
	 	 	 x *= invLength;
	 	 	 y *= invLength;
	 	 	 z *= invLength;
	 	 }
	 	 return *this;
	 }
	 
	 FORCEINLINE JDVector3 
		JDVector3::getnormalize() {
 	 	JDVector3 result = *this;
 	 	float mod = squareMagnitude();
 	 	if (mod == 0)
 	 	{
 	 	 	result.x = 0;
 	 	 	result.y = 0;
 	 	 	result.z = 0;
 	 	}
 	 	else
 	 	{
 	 	 	float invLength = 1.0f / sqrtf(mod);
 	 	 	result.x = x * invLength;
 	 	 	result.y = y * invLength;
 	 	 	result.z = z * invLength;
 	 	}
 	 	return result;
	 }
	 
	 FORCEINLINE float 
		JDVector3::dot(const JDVector3& vec) const {
	 	 return (x * vec.x + y * vec.y + z * vec.z);
	 }
	 
	 FORCEINLINE JDVector3 
		JDVector3::cross(const JDVector3& vec) const {
 	 	return JDVector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	 }
}
