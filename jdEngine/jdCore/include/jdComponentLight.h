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
#include "jdVector4.h"

namespace jdEngineSDK {
		class JD_CORE_EXPORT CLight : public Component
		{
		 public:
    /**
     * @brief Default constructor
     */
    CLight() {
      m_type = COMPONENT_TYPE::LIGHT;
    };

    /**
     * @brief Default destructor
     */
    ~CLight() {};

    /**
     * @brief to set the id in the array of lights
     * @param id is its id in the arrat
     */
    void
    setIdArray(uint32 id) {
      m_idArray = id;
    };

    /**
     * @brief to get the id in the array of lights
     */
    uint32
    getIdArray() {
      return m_idArray;
    };

    /**
     * @brief funtion to upadate  the matrix
     * @param deltatime is the elpase timen between frames
     */
    void
    Update(const float& deltatime) override;

    /**
     * @brief to set the type of light
     * @param type is its the type of light
     * @note 0 is directional, 1 is point, 2 is spot
     */
    void
    setTypeLight(uint32 type) {
      m_typeLight = type;
    };

    /**
     * @brief to get the type of light
     */
    uint32
    getTypeLight() {
      return m_typeLight;
    };

    /**
     * @brief to get the direction of light
     */
    JDVector4
    getDirection() {
      return m_direction;
    };

    /**
     * @brief to get the position of light
     */
    JDVector4
    getPosition() {
      return m_position;
    };

    /**
     * @brief the light color
     */
    JDVector4 m_color = { 1,1,1,1 };
		 private:

    /**
     * @brief the id in the array of lights
     */
    uint32 m_idArray = 0;

    /**
     * @brief the light direction
     */
    JDVector4 m_direction;

    /**
     * @brief the light position
     */
    JDVector4 m_position;

    /**
     * @brief the light type
     * @note 0 is directional, 1 is point, 2 is spot
     */
    uint32 m_typeLight = 0;
		};
}