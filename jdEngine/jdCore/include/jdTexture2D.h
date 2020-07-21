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

namespace jdEngineSDK {

  class Texture2D
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
    getTexture() { return nullptr; };

    /**
     * @brief virtual function to release the texture
     */
    virtual void
    release() {};

  };
}
