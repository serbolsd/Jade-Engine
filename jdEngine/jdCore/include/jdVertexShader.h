/*****************************************************************************/
/**
 * @file    jdVertexShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual VertexShader class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class VertexShader
  {
   public:
   /**
    * @brief Default constructor
    */
    VertexShader() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~VertexShader() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}
