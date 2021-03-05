/*****************************************************************************/
/**
 * @file    jdGeometryShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    01/FEB/2021
 * @brief   Virtual Geometry Shader class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class GeometryShader
  {
   public:
    /**
     * @brief Default constructor
     */
    GeometryShader() = default;
    
    /**
     * @brief virtual destructor
     */
    virtual ~GeometryShader() {};

    /**
     * @brief virtual function to release
     */
    virtual void
      release() {};
  };
}