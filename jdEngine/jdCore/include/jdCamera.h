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

#include "jdMatrix4.h"
#include "jdVector3.h"

namespace jdEngineSDK {
		using std::move;

		class Camera
		{
		public:
				Camera();
				Camera(String name, 
						     JDVector3 eye = JDVector3(0.0f), 
						     JDVector3 up = JDVector3(0.0f), 
						     JDVector3 at = JDVector3(0.0f));

				~Camera();

				void
				setID(uint32 id) {
						m_id = id;
				};

				uint32
				getID() {
						return m_id;
				};

				void
				setName(String name) {
						m_name = move(name);
				};

				const String&
				getName() {
						return m_name;
				};

				const JDMatrix4&
				getMatrixView() {
						return m_view;
				};

				const JDMatrix4&
				getMatrixProjection() {
						return m_projection;
				};

				const JDVector3&
				getFrontVector() {
						return m_front;
				};

				const JDVector3&
				getUpVector() {
						return m_up;
				};

    const JDVector3&
				getRightVector() {
						return m_right;
				};

    const JDVector3&
				getRightVector() {
						return m_position;
				};

				void
				traslate(const float& x, const float& y, const float& z);

				void
				rotate(const float& anglex, const float& angley, const float& anglez);

		private:
				String m_name;
				uint32 m_id;
				uint32 m_hash;

				JDMatrix4 m_view;
				JDMatrix4 m_projection;

				JDVector3 m_front;
				JDVector3 m_up;
				JDVector3 m_right;
				JDVector3 m_position;


		};
}
