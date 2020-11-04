/*****************************************************************************/
/**
 * @file    jdTexture2D.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    02/JUN/2020
 * @brief   Virtual Rendet Target class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include "jdResource.h"

namespace jdEngineSDK {

  class Texture2D : public Resource
  {
   public:
    /**
     * @brief default constructor
     */
    Texture2D() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~Texture2D() {};

    /**
     * @brief get pointer to texture
     * @return pointer to texture
     */
    virtual void*
    getTexture(uint32 index = 1) { return nullptr; };

    ///**
    // * @brief virtual function to release the texture
    // */
    //virtual void
    //release() {};
    void 
    setName(const String& name) {
      m_name = name;
    }

    String
    getName() const{
      return m_name;
    };

    virtual void
    release() {};
   protected:
    String m_name = "defaultName";
  };
}
