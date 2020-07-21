/*****************************************************************************/
/**
 * @file    DeviceContext.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual Device Context class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

#include <jdModulo.h>

namespace jdEngineSDK {

  class  DeviceContext
  {
   public:
    /**
     * @brief Default constructor
     */
    DeviceContext() = default;
    
    /**
     * @brief Virtual destructor
     */
    virtual ~DeviceContext() {};

  };
}
