/*****************************************************************************/
/**
 * @file    jdVertexBuffer.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUL/2020
 * @brief   Virtual VertexBuffer class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class VertexBuffer
  {
   public:
    /**
     * @brief Default constructor
     */
    VertexBuffer() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~VertexBuffer() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}
