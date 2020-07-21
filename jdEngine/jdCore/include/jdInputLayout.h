/*****************************************************************************/
/**
 * @file    jdInputLayout.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    9/JUL/2020
 * @brief   Virtual InputLayout class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class InputLayout
  {
  public:
    /**
     * @brief Default constructor
     */
    InputLayout() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~InputLayout() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}
