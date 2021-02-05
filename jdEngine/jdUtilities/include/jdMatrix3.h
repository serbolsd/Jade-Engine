/*****************************************************************************/
/**
 * @file    jdMatrix3.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUN/2020
 * @brief   Structure for float matrix 3x3 in row mallet.
 *
 * Structure for float matrix 3x3 row mallet.
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"
#include "jdRadian.h"

#include <math.h>
#include <iomanip>

namespace jdEngineSDK
{
  class JD_UTILITY_EXPORT JDMatrix3
  {
/*****************************************************************************/
/**
 * Constructors
 */
/*****************************************************************************/
   public:
    /**
     * @brief Default constructor with a identity matrix.
     */
    JDMatrix3();
    
    /**
     * @brief Default constructor with a array.
     * @param src is a array with the data
     */
    JDMatrix3(const float src[9]);
    
    /**
     * @brief Default constructor with floats.
     */
    JDMatrix3(float m0, float m1, float m2, 
              float m3, float m4, float m5, 
              float m6, float m7, float m8);
    
    /**
     * @brief Default constructor with 3 jdvector3.
     */
    JDMatrix3(const JDVector3& m012,        
              const JDVector3& m345, 
              const JDVector3& m678);

/*****************************************************************************/
/**
 * Operators
 */
/*****************************************************************************/
    /**
     * @brief Negative operator
     * @return A new matrix negative of this matrix
     */
    JDMatrix3
    operator-() const;
    
    /**
     * @brief Positive operator
     * @return A new matrix positive of this matrix
     */
    JDMatrix3
    operator+() const;
    
    /**
     * @brief add two matrix
     * @param mat is the other matrix to add
     * @return the addition of this vector with other
     */
    JDMatrix3     
    operator+(const JDMatrix3& mat) const;
    
    /**
     * @brief substraction two matrix
     * @param mat is the other matrix to subtrac
     * @return the substraction of this vector with other
     */
    JDMatrix3    
    operator-(const JDMatrix3& mat) const;
    
    /**
     * @brief add this matrix with other
     * @param mat is the other matrix to add
     * @return the addition of this matrix with other in this matrix
     */
    JDMatrix3& 
    operator+=(const JDMatrix3& mat);
    
    /**
     * @brief substraction this matrix with an other matrix
     * @param mat is the other matrix to subtrac
     * @return the substraction of this matrix with other in this matrix
     */
    JDMatrix3& 
    operator-=(const JDMatrix3& mat);      
    
    /**
     * @brief multiply this matrix with a vector3
     * @param vec is the vector to multiply this matirx
     * @return the multiplication of this matrix with a vector3
     */
    JDVector3    
    operator*(const JDVector3& vec) const;
    
    /**
     * @brief multiply this matrix with a vector3
     * @param mat is the mat to multiply this matirx
     * @return the multiplication of this matrix with oyher matrix
     */
    JDMatrix3  
    operator*(const JDMatrix3& mat) const;
    
    /**
     * @brief multiply this matirx with an other matrix
     * @param mat is the matrix to multiply this matrix
     * @return the multiplication of this matrix with a matrix in this matrix
     */
    JDMatrix3&
    operator*=(const JDMatrix3& mat);  
    
    /**
     * @brief compare this matrix with an other matrix.
     * @param mat is matrix what to compare vector by.
     * @return true if all components are equal
     */
    bool 
    operator==(const JDMatrix3& mat) const; 
    /**
     * @brief compare this matrix with an other matrix.
     * @param mat is matrix what to compare matrix by.
     * @return true if one component is diferent
     */
    bool    
    operator!=(const JDMatrix3& mat) const; 
    
    /**
     * @brief get matrix value on index
     * @param index index of value.
     * @return a const of the value on index
     * @note if index less 0 return value on index 0, if index greater 8
     *       return value on index 8
     */
    float 
    operator[](uint32 index) const; 
    
    /**
     * @brief get matrix value on index
     * @param index index of value.
     * @return the value on index
     * @note if index less 0 return value on index 0, if index greater 8
     *       return value on index 8
     */
    float&
    operator[](uint32 index);


/*****************************************************************************/
/**
 * Friends
 */
/*****************************************************************************/

    /**
     * @brief subtrac two matrix
     * @param mat is the matrix with which to subtract
     * @return the subtraction
     */
    friend 
    JDMatrix3 operator-(const JDMatrix3& mat);
    
    friend std::ostream& 
    operator<<(std::ostream& os, const JDMatrix3& mat);

/*****************************************************************************/
/**
 * Functions
 */
/*****************************************************************************/

    /**
     * @brief get the matrix´s data
     * @return the matrix's data
     */
    const float*
    get() const;
    
    /**
     * @brief get the matrix´s data
     * @return the matrix's data
     */
    float
    getDeterminant() const;
    
    /**
     * @brief transform this matrix to matrix identity
     * @return this matrix as a identity matrix
     */
    JDMatrix3&
    identity();
    
    /**
     * @brief transform this matrix to its transpose
     * @return this matrix as a its traspose form
     */
    JDMatrix3&
    transpose();
    
    /**
     * @brief get the transpose of this matrix
     * @return a const of this matrix transposed
     */
    JDMatrix3
    getTranspose() const;
    
    /**
     * @brief transform this matrix to its invert
     * @return this matrix as a its invert form
     */
    JDMatrix3&
    invert();
    
    /**
     * @brief get the invert of this matrix
     * @return  a const of this matrix inverted
     */
    JDMatrix3
    getInvert() const;
    
    /**
     * @brief traslate this matrix on x and y cordinates
     * @param x value to traslate on x cordinate
     * @param y value to traslate on y cordinate
     * @return this matrix traslated
     */
    JDMatrix3&
    translate(float x, float y);
    
    /**
     * @brief traslate this matrix on x and y cordinates
     * @param vec value to traslate on x, y cordinate
     * @return this matrix traslated
     */
    JDMatrix3&
    translate(const JDVector2& vec);
    
    /**
     * @brief rotate this matrix
     * @param angle angle on radians
     * @return this matrix rotated
     */
    JDMatrix3&
    rotate(const Radian& angleRadian);
    
    /**
     * @brief scale this matrix
     * @param scale is the value to scale this matrix
     * @return this matrix scaled
     */
    JDMatrix3&
    scale(float scale);
    
    /**
     * @brief scale this matrix
     * @param sx is the value to scale on x axis this matrix
     * @param sy is the value to scale on y axis this matrix
     * @return this matrix scaled
     */
    JDMatrix3&
    scale(float sx, float sy);

/*****************************************************************************/
/**
 * Members
 */
/*****************************************************************************/
   public:
    /**
     * @brief union of diferents estructs
     */
    union
    {
      struct
      {
        /**
         * @brief array of 9 floats
         */
        float m[9];
      };
      struct
      {
      		/**
      		 * @brief 9 floats
      		 */
      		float m_00, m_01, m_02,
      				m_10, m_11, m_12,
      				m_20, m_21, m_22;
      };
      /**
       * @brief 3 arrays with 3 arrays of floats
       */
      float M[3][3];
    };
  };

/*****************************************************************************/
/**
 * Extern Functions
 */
/*****************************************************************************/
  
  /**
   * @brief Transpose a matrix
   * @param mat is the matrix to traspose
   * @return a new matrix with the data of the param matrix transpose
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Transpose(const JDMatrix3& mat);
  
  /**
   * @brief invert a matrix
   * @param mat is the matrix to invert
   * @return a new matrix with the data of the param matrix invert
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Invert(const JDMatrix3& mat);
  
  /**
   * @brief get identity matrix
   * @return a new matrix with the data of the param matrix invert
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Identity();
  
  /**
   * @brief get translate matrix
   * @param x is for x axis
   * @param y is for y axis
   * @return a matrix traslalate
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Translate(float x, float y);		  
  
  /**
   * @brief get translate matrix
   * @param vec is for x, y axis
   * @return a matrix traslalate
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Translate(const JDVector2& vec);
  
  /**
   * @brief get a rotation matrix
   * @param angle angle on radians
   * @return a rotation matrix
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Rotate(const Radian& angleRadian);            
  
  /**
   * @brief get a scale matrix
   * @param scale is the value to scale the matrix
   * @return a scale matrix
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Scale(float scale);             
  
  /**
   * @brief scale a matrix
   * @param sx is the value to scale on x axis the matrix
   * @param sy is the value to scale on y axis the matrix
   * @return a scale a matrix
   */
  JD_UTILITY_EXPORT extern JDMatrix3
  matrix3Scale(float sx, float sy);			   
}
