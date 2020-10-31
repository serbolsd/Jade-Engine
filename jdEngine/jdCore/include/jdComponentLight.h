/*****************************************************************************/
/**
 * @file    jdComponentLight.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    28/OCT/2020
 * @brief   Class light component
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include "jdComponent.h"
namespace jdEngineSDK {
		class CLight : public Component
		{
		public:
    CLight() {
      m_type = COMPONENT_TYPE::LIGHT;
    };
    ~CLight() {};

    void
    setIdArray(uint32 id) {
      m_idArray = id;
    };

    uint32
    getIdArray() {
      return m_idArray;
    };
		private:
    uint32 m_idArray = 0;
		};
}