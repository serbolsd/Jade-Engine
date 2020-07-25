/*****************************************************************************/
/**
 * @file    jdPixelShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual PixelShader class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class PixelShader
  {
   public:
    /**
     * @brief Default constructor
     */
    PixelShader() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~PixelShader() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}