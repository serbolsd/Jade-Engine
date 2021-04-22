#pragma once
/*****************************************************************************/
/**
 * @file    jdCamera.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/OCT/2020
 * @brief   class for camera Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <jdMatrix4.h>
#include <jdVector3.h>
#include <jdRadian.h>
#include <jdDegree.h>

#include "jdComponent.h"

namespace jdEngineSDK {
		using std::move;
		namespace CAMERA_PROJECTION_TYPE {
				enum E {
						UNKNOWTYPE = 0,
						PERSPECTIVE,
						HORTOGRAPHIC,
						NUMPROJECTIONTYPES
				};
		}

		class JD_CORE_EXPORT Camera : public Component
		{
				friend class CameraManager;
		public:
				Camera() = default;

				Camera(String name,
						     uint32 id,
						     const JDVector3& eye = JDVector3(0.0f, 0.0f, 0.0f),
						     const JDVector3& up = JDVector3(0.0f,1.0f,0.0f),
						     const JDVector3& at = JDVector3(0.0f,0.0f,1.0f),
						     const float& _near=0.01f,
						     const float& _far=1000.0f,
						     const Radian& fovAngle = Radian(Math::PI/4),
						     const float& aspcRatio = 1.777777777777778f,
						     CAMERA_PROJECTION_TYPE::E projType = CAMERA_PROJECTION_TYPE::PERSPECTIVE);

				~Camera() {};

				void
    Update(const float& deltatime) override;

				/**
					* @brief fuction to set id to the camera
					* @param id is the id to set
					*/
				void
				setID(uint32 id) {
						m_id = id;
				};

				/**
					* @brief fuction to get the camera큦 id
					*/
				uint32
				getID() {
						return m_id;
				};

				//void
				//setName(String name) {
				//		m_name = move(name);
				//};

				/**
					* @brief fuction to get the camera큦 name
					*/
				const String&
				getName() {
						return m_name;
				};

				/**
					* @brief fuction to get the camera큦 view matrix
					*/
				const JDMatrix4&
				getMatrixView() {
						return m_view;
				};

				/**
					* @brief fuction to get the camera큦 projection matrix
					*/
				const JDMatrix4&
				getMatrixProjection() {
						return m_projection;
				};

				/**
					* @brief fuction to get the camera큦 front vector
					*/
				const JDVector3&
				getFrontVector() {
						return m_front;
				};

				/**
					* @brief fuction to get the camera큦 up vector
					*/
				const JDVector3&
				getUpVector() {
						return m_up;
				};

				/**
					* @brief fuction to get the camera큦 right vector
					*/
    const JDVector3&
				getRightVector() {
						return m_right;
				};

				/**
					* @brief fuction to get the camera큦 position vector
					*/
    const JDVector4&
				getPositionVector() {
						return m_position;
				};

				/**
					* @brief fuction traslate the camera
					*/
				void
				traslate(const float& x, const float& y, const float& z);

				/**
					* @brief fuction rotation the camera
					*/
				void
				rotate(const Radian& anglexRad, const Radian& angleyRad);

				/**
					* @brief fuction to resize the camera
					*/
				void
				resize(int32 width, int32 height);

				/**
					* @brief fuction to resize the camera
					*/
				void
				adjustNearAndFar(const float& _near, const float& _far);

				float 
				getNear() {
						return m_near;
				}

    float 
				getFar() {
						return m_far;
				}

				float 
				getAspectRatio() {
						return m_aspecRatio;
				}

				Radian
				getFovAngle() {
						return m_fovAngle;
				}

				float 
				getWidth() {
						return m_width;
				}

				float 
				getHeight() {
						return m_height;
				}

				CAMERA_PROJECTION_TYPE::E 
				gerProjectionType() {
						return m_projType;
				};

		private:

				/**
					* @brief the camera큦 name
					*/
				String m_name;

				/**
					* @brief the camera큦 id
					*/
				uint32 m_id;

				/**
					* @brief the camera큦 near
					*/
				float m_near;

				/**
					* @brief the camera큦 near
					*/
				float m_far;

				/**
					* @brief the camera큦 fovAngle
					*/
				Radian m_fovAngle;

				/**
					* @brief the camera큦 fovAngle
					*/
				float m_aspecRatio;

				/**
					* @brief the camera큦 fovAngle
					*/
				float m_width;

				/**
					* @brief the camera큦 fovAngle
					*/
				float m_height;

				/**
					* @brief the camera큦 view
					*/
				JDMatrix4 m_view;

				/**
					* @brief the camera큦 projection
					*/
				JDMatrix4 m_projection;

				/**
					* @brief the camera큦 front
					*/
				JDVector3 m_front;

				/**
					* @brief the camera큦 up
					*/
				JDVector3 m_up;

				/**
					* @brief the camera큦 right
					*/
				JDVector3 m_right;

				/**
					* @brief the camera큦 position
					*/
				JDVector4 m_position;

				CAMERA_PROJECTION_TYPE::E m_projType;
		};
}
