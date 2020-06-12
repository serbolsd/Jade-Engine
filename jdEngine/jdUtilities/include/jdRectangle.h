/*****************************************************************************/
/**
	* @file    jdRectangle.h
	* @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
	* @date    11/JUN/2020
	* @brief   A basic Rectangle
	*
	* The rectangle had base, height, position, rotatio, scale
	* 
	*
	* @bug	    No known bugs.
	*/
	/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"
#include "jdMatrix3.h"
#include "jdRadian.h"

namespace jdEngineSDK {

		class JD_UTILITY_EXPORT Rectangle
		{
	 	public:
///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////
				/**
					* @brief Default constructor rectangle.
					*/
					Rectangle() = default;

				/**
					* @brief Constructor rectangle given a base and height.
					* @param base is for base component
					* @param height is for heigh component
					* @note initialice position on 0, 0, 0 cordinates
					*/
				Rectangle(const float& base, const float& height) : 
																										m_base(base), m_height(height), m_position(0.0f),
						                    m_scale(0.0f),m_rotation(0.0f) {};

				Rectangle(const Rectangle& rectangle);
///////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////
			public:
				/**
					* @brief Equal this rectangle to another
					* @param rectangle is the other rectangle to equalize.
					* @return the values of the another rectangle in this vector
					*/
				Rectangle&
				operator=(const Rectangle& rectangle);

				/**
	    * @brief Gets the result of comparation of this vector with another.
	    * @param vec is the other vector to compare this by.
	    * @return true if are equal, else return false.
	    */
				FORCEINLINE bool
				operator==(const Rectangle& rectangle) const;

				/**
					* @brief Gets the result of comparation of this vector with another.
					* @param vec is the other vector to compare this by.
					* @return true if are diferent, else return false.
					*/
				FORCEINLINE bool
				operator!=(const Rectangle& vec) const;

				/**
					* @brief Gets the result of comparation of this vector with another.
					* @param vec is the other vector to compare this by.
					* @return true if a component in this vector is less than the other vector, else return false.
					*/
				FORCEINLINE bool
				operator<(const Rectangle& vec) const;

				/**
					* @brief Gets the result of comparation of this vector with another.
					* @param vec is the other vector to compare this by.
					* @return true if a component in this vector is greater than the other vector, else return false.
					*/
				FORCEINLINE bool
				operator>(const Rectangle& vec) const;


///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////
				/**
					* @brief set rectangle's position
					* @param pos vector with the cordinates x, y, z
					*/
				FORCEINLINE void
				setPosition(const JDVector3& pos) {
						m_position = pos;
				}

				/**
					* @brief set rectangle's position
					* @param posX for x cordinate
					* @param posY for y cordinate
					* @param posZ for z cordinate
					*/
				FORCEINLINE void
						setPosition(const float& posX, const float& posY, const float& posZ) {
						m_position = {posX,posY,posZ};
				}

				/**
					* @brief Get rectangle's position
					* @return a vector with the position
					*/
				FORCEINLINE JDVector3
				getPosition() const {
						return m_position;
				}

				FORCEINLINE void
				setBase(const float& base) {
						m_base = base;
				}

				FORCEINLINE float
				getBase() const {
						return m_base;
				}

				FORCEINLINE void
				setHeight(const float& height) {
						m_height = height;
				}

				FORCEINLINE float
				getHeight() const {
						return m_height;
				}
				
				FORCEINLINE float
				getArea() const {
						return m_height * m_base * m_scale;
				}

				FORCEINLINE float
				getPerimeter() const {
						return (m_height * 2 * m_scale) + (m_base * 2 * m_scale);
				}

				/**
					* @brief Set rectangle's Scale
					* @param scale is the value to scale
					*/
				FORCEINLINE void
				setScale(const float& scale) {
						m_scale = scale;
				}


				FORCEINLINE float
				getScale() const {
						return m_scale;
				}

				/**
					* @brief Set rectangle's Rotation
					* @param angleRadian is the angle on radians
					*/
				FORCEINLINE void
						setRotation(const Radian& angleRadian) {
						m_rotation = angleRadian;
				}

				/**
					* @brief get rectangle's rotation angle as a radians
					* @return a flaoat with the rectangle's rotation angle as a radians
					*/
				FORCEINLINE float
				getRotationRadians() const {
						return m_rotation.valueRadians();
				}

				/**
					* @brief get rectangle's rotation angle as a degrees
					* @return a flaoat with the rectangle's rotation angle as a degrees
					*/
				FORCEINLINE float
				getRotationDegrees() const {
						return m_rotation.valueDegrees();
				}

///////////////////////////////////////////////////////////////////////////////
// Members
///////////////////////////////////////////////////////////////////////////////
		 private:
				float m_scale;

				Radian m_rotation;

				JDVector3 m_position;
		
				float m_base;		
				float m_height;

		};
}
