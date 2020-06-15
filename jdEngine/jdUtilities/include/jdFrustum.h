/*****************************************************************************/
/**
	* @file    jdMatrix4.h
	* @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
	* @date    14/JUN/2020
	* @brief   Structure for frustum.
	*
	* the frustum had the planes left, right, top, botton, near(back), far(front)
	*
	* @bug     No known bugs.
	*/
#pragma once

#include "jdPlatformDefines.h"
#include "jdPlane.h"
#include "jdMatrix4.h"

namespace jdEngineSDK
{
		class JD_UTILITY_EXPORT Frustum
		{
		public:
///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////
				/**
					* @brief Default constructor with a identity matrix.
					*/
				Frustum()=default;
				/**
					* @brief constructor with view projection matrix.
					* @param viewProjMat is the view projection matrix
					* @note calculate the planes left, right, botton, top, back, front
					*/
				Frustum(const JDMatrix4& viewProjMat);

				/**
					* @brief constructor with other furstum.
					* @param Frustum is the other frustum
					*/
				Frustum(const Frustum& other);
				
				/**
					* @brief get left plane of frustum.
					* @return left plane
					*/
				FORCEINLINE Plane
				getLeftPlane() const {
						return m_left;
				};

				/**
					* @brief get right plane of frustum.
					* @return right plane
					*/
				FORCEINLINE Plane
				getRightPlane() const {
						return m_right;
				};

				/**
					* @brief get botton plane of frustum.
					* @return botton plane
					*/
				FORCEINLINE Plane
				getBottonPlane() const {
						return m_bottom;
				};

				/**
					* @brief get top plane of frustum.
					* @return top plane
					*/
				FORCEINLINE Plane
				getTopPlane() const {
						return m_top;
				};

				/**
					* @brief get back plane of frustum.
					* @return back plane
					*/
				FORCEINLINE Plane
				getBackPlane() const {
						return m_back;
				};

				/**
				 * @brief get front plane of frustum.
				 * @return front plane
				 */
				FORCEINLINE Plane
				getFrontPlane() const {
						return m_front;
				};


		private:
				Plane m_left;
				Plane m_right;
				Plane m_bottom;
				Plane m_top;
				Plane m_back;
				Plane m_front;
		};
}