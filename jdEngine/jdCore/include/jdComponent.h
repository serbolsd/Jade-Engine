/*****************************************************************************/
/**
 * @file    jdGameObject.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    03/AGO/2020
 * @brief   Virtual class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  namespace COMPONENT_TYPE {
    enum E {
      TRANSFORM = 0,
      RENDERMODEL,
      TEXTURE,
      BOXCOLLIDER,
      SPHERECOLLIDRE,
      SOUND,
      SPRITE,
      CAMERA,
      LIGHT,
      RIGIDBODY,
      KNUMCOMPONENTS
    };
  };

  class GameObject;

  class Component
  {
   public:
    Component() = default;
    ~Component() {};

    COMPONENT_TYPE::E
    getType() const {
      return m_type;
    }

    virtual void
    Update(const float& /*deltatime*/) {};

    void
    setGameObject(GameObject* myObject){
      m_myObject = myObject;
    }

   protected:
    COMPONENT_TYPE::E m_type;

    GameObject* m_myObject = nullptr;
  };
}
