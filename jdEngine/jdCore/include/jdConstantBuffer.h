/*****************************************************************************/
/**
 * @file    jdContantBuffer.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUL/2020
 * @brief   Virtual ContantBuffer class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class ConstantBuffer
  {
   public:
    /**
     * @brief Default constructor
     */
     ConstantBuffer() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~ConstantBuffer() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}
