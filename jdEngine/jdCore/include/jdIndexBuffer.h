/*****************************************************************************/
/**
 * @file    jdIndexBuffer.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUL/2020
 * @brief   Virtual IndexBuffer class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class IndexBuffer
  {
   public:
    /**
     * @brief Default constructor
     */
    IndexBuffer() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~IndexBuffer() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}
